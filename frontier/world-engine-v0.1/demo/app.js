import { renderCharts } from "./charts.js";
import { applyDragonPose } from "./dragon.js";
import { causeFor, loadEvidence, percent, signedDegrees, sourceLabel } from "./trace.js";

const $ = (id) => document.getElementById(id);
const query = new URLSearchParams(window.location.search);
const liveMode = query.get("live") === "1";
const freezeLiveFrame = query.get("freeze") === "1";
const evidenceSelection = query.get("evidence");
const state = { frames: [], equalizerSamples: [], verification: null, sequence: 365, playing: false, lastTick: 0, lastLiveSequence: -1, lastLoad: 0 };
const frameDuration = 1000 / 30;

const elements = {
  shell: document.querySelector(".app-shell"),
  signalTitle: $("signalTitle"), signalSubtitle: $("signalSubtitle"),
  strengthLabel: $("strengthLabel"), variabilityLabel: $("variabilityLabel"), noveltyLabel: $("noveltyLabel"),
  embodimentSubtitle: $("embodimentSubtitle"), liveModeLink: $("liveModeLink"),
  strengthValue: $("strengthValue"), variabilityValue: $("variabilityValue"), noveltyValue: $("noveltyValue"),
  railStrength: $("railStrength"), railVariability: $("railVariability"), railNovelty: $("railNovelty"),
  behaviorValue: $("behaviorValue"), inputLabel: $("inputLabel"), causeText: $("causeText"),
  wingAngle: $("wingAngle"), tailAngle: $("tailAngle"), railWing: $("railWing"), railTail: $("railTail"),
  sourceValue: $("sourceValue"), freshnessValue: $("freshnessValue"), sequenceValue: $("sequenceValue"),
  frameHashValue: $("frameHashValue"), traceHashValue: $("traceHashValue"), languageValue: $("languageValue"),
  divergenceValue: $("divergenceValue"), selectedSequence: $("selectedSequence"), selectedTime: $("selectedTime"),
  chartPlayhead: $("chartPlayhead"), timelinePlayhead: $("timelinePlayhead"), playheadLabel: $("playheadLabel"),
  replayButton: $("replayButton"), pauseButton: $("pauseButton"), resetButton: $("resetButton"),
  senderStopButton: $("senderStopButton"), timeline: $("timeline"), errorScreen: $("errorScreen"), errorMessage: $("errorMessage"),
  equalizerBars: $("equalizerBars"), equalizerCaption: $("equalizerCaption"), loadEqualizer: $("loadEqualizer"),
};

function clampSequence(sequence) {
  return Math.max(0, Math.min(state.frames.length - 1, Math.round(sequence)));
}

function setPlaying(playing) {
  state.playing = playing;
  elements.pauseButton.setAttribute("aria-pressed", String(playing));
  elements.pauseButton.setAttribute("aria-label", playing ? "Pause replay" : "Resume replay");
  elements.shell.classList.toggle("is-playing", playing);
  if (playing) state.lastTick = performance.now();
}

function equalizerLoad(sample) {
  if (typeof sample?.load === "number") return sample.load;
  if (typeof sample?.features_q16?.strength === "number") return sample.features_q16.strength / 65_535;
  return null;
}

function renderEqualizer(samples, selectedSequence) {
  const valid = samples.filter((sample) => sample && sample.valid !== false && equalizerLoad(sample) !== null);
  const selectedIndex = valid.findIndex((sample) => sample.sequence === selectedSequence);
  const end = selectedIndex >= 0 ? selectedIndex + 1 : valid.length;
  const visible = valid.slice(Math.max(0, end - 32), end);
  const fragment = document.createDocumentFragment();

  for (let index = 0; index < 32; index += 1) {
    const sample = visible[index - (32 - visible.length)];
    const bar = document.createElement("span");
    bar.className = "equalizer-bar";
    if (!sample) {
      bar.classList.add("empty");
    } else {
      const load = Math.min(1, Math.max(0, equalizerLoad(sample)));
      const behavior = String(sample.behavior || "CALM").toLowerCase();
      const throughput = sample.throughput_mbps ?? sample.network_load?.throughput_mbps;
      bar.classList.add(behavior);
      if (sample.sequence === selectedSequence) bar.classList.add("selected");
      bar.style.setProperty("--level", load.toFixed(4));
      bar.title = `seq ${sample.sequence} · ${(load * 100).toFixed(0)}% load${Number.isFinite(throughput) ? ` · ${throughput.toFixed(2)} Mbps` : ""} · ${sample.behavior}`;
    }
    fragment.append(bar);
  }

  elements.equalizerBars.replaceChildren(fragment);
  elements.equalizerCaption.textContent = `${visible.length} measured samples · time domain`;
  elements.loadEqualizer.setAttribute(
    "aria-label",
    `${visible.length} recent measured host network-load samples over time. This is not an RF-frequency spectrum.`,
  );
}

function render(sequence) {
  state.sequence = clampSequence(sequence);
  const frame = state.frames[state.sequence];
  const strength = `${percent(frame.features_q16.strength)}%`;
  const variability = frame.network_load
    ? `${frame.network_load.throughput_mbps.toFixed(2)} Mbps`
    : `${percent(frame.features_q16.variability)}%`;
  const novelty = `${percent(frame.features_q16.novelty)}%`;
  const wing = signedDegrees(frame.pose.wing_l_roll_mdeg);
  const tail = signedDegrees(frame.pose.tail3_yaw_mdeg);
  const progress = (state.sequence / (state.frames.length - 1)) * 100;

  elements.shell.dataset.state = frame.behavior;
  elements.strengthValue.textContent = elements.railStrength.textContent = strength;
  elements.variabilityValue.textContent = elements.railVariability.textContent = variability;
  elements.noveltyValue.textContent = elements.railNovelty.textContent = novelty;
  elements.behaviorValue.textContent = frame.behavior;
  elements.inputLabel.textContent = frame.network_load
    ? "Live Windows network byte counters"
    : sourceLabel(frame.source);
  elements.causeText.textContent = frame.network_load
    ? `${frame.network_load.throughput_mbps.toFixed(2)} Mbps / ${frame.network_load.capacity_mbps.toFixed(1)} Mbps = ${strength} load. The same load value scales the procedural joint amplitude.`
    : causeFor(frame);
  elements.wingAngle.textContent = elements.railWing.textContent = wing;
  elements.tailAngle.textContent = elements.railTail.textContent = tail;
  elements.sourceValue.textContent = frame.source;
  const freshness = frame.network_load ? Math.max(0, Date.now() - frame.timestamp_unix_ms) : frame.freshness_ms;
  elements.freshnessValue.textContent = freezeLiveFrame
    ? "CAPTURED"
    : freshness > 1_500 ? "STALE" : `${freshness} ms`;
  elements.sequenceValue.textContent = String(frame.sequence).padStart(3, "0");
  elements.frameHashValue.textContent = frame.frame_hash.slice(0, 8);
  elements.selectedSequence.textContent = `seq ${frame.sequence}`;
  elements.selectedTime.textContent = frame.network_load ? "live sample" : `${(frame.tick / 60).toFixed(2)} s engine time`;
  elements.playheadLabel.textContent = frame.sequence;
  elements.chartPlayhead.style.left = `${progress}%`;
  elements.timelinePlayhead.style.left = `${progress}%`;
  elements.timelinePlayhead.setAttribute("aria-label", `Selected sequence ${frame.sequence}, ${frame.behavior}`);
  elements.timeline.setAttribute("aria-valuenow", String(frame.sequence));
  applyDragonPose(frame);
  if (liveMode) renderEqualizer(state.equalizerSamples.length ? state.equalizerSamples : state.frames, frame.sequence);
}

function livePose(sample) {
  const load = sample.load;
  const phase = sample.sequence * 0.42;
  const breath = 2_000 + load * 48_000;
  const wingSpread = 3_000 + load * 25_000;
  const flap = 2_000 + load * 5_000;
  const tailCurl = load * 18_000;
  const tailWave = 1_500 + load * 4_000;
  return {
    root_y_um: Math.round(Math.sin(phase) * breath),
    root_roll_mdeg: Math.round(Math.sin(phase * 0.43) * (500 + load * 3_500)),
    neck1_pitch_mdeg: Math.round(-2_000 - load * 7_000 + Math.sin(phase * 0.61) * 2_000),
    neck2_pitch_mdeg: Math.round(1_000 + load * 5_000 + Math.sin(phase * 0.77) * 1_500),
    head_yaw_mdeg: Math.round(Math.sin(phase * 0.83) * (1_000 + load * 8_000)),
    wing_l_roll_mdeg: Math.round(wingSpread + Math.sin(phase * 1.31) * flap),
    wing_r_roll_mdeg: Math.round(-wingSpread - Math.sin(phase * 1.31) * flap),
    tail1_yaw_mdeg: Math.round(tailCurl * 0.45 + Math.sin(phase * 0.71) * tailWave),
    tail2_yaw_mdeg: Math.round(tailCurl * 0.72 + Math.sin(phase * 0.71 + 0.7) * tailWave),
    tail3_yaw_mdeg: Math.round(tailCurl + Math.sin(phase * 0.71 + 1.4) * tailWave),
  };
}

function frameFromLiveSample(sample) {
  const delta = Math.min(Math.abs(sample.load - state.lastLoad), 1);
  state.lastLoad = sample.load;
  return {
    schema: "world.network-load-pose/0.1",
    sequence: sample.sequence,
    tick: sample.sequence,
    timestamp_unix_ms: sample.timestamp_unix_ms,
    source: sample.source,
    valid: sample.valid,
    freshness_ms: 0,
    behavior: sample.behavior,
    features_q16: {
      strength: sample.load_q16,
      variability: sample.load_q16,
      novelty: Math.round(delta * 65_535),
    },
    pose: livePose(sample),
    frame_hash: sample.frame_hash,
    network_load: {
      throughput_mbps: sample.throughput_mbps,
      capacity_mbps: sample.capacity_mbps,
    },
  };
}

function configureLiveView() {
  document.body.classList.add("live-mode");
  document.querySelector(".transport").hidden = true;
  document.querySelector(".timeline-section").hidden = true;
  document.querySelector('.mode-switch a[href="index.html"]').classList.remove("active");
  document.querySelector('.mode-switch a[href="index.html"]').removeAttribute("aria-current");
  elements.liveModeLink.classList.add("active");
  elements.liveModeLink.setAttribute("aria-current", "page");
  elements.signalTitle.textContent = "Measured host network load";
  elements.signalSubtitle.textContent = freezeLiveFrame
    ? `WINDOWS INTERFACE BYTES · FROZEN ${evidenceSelection ? evidenceSelection.toUpperCase() + " LOAD " : ""}EVIDENCE FRAME`
    : "WINDOWS INTERFACE BYTES · 250 ms samples";
  elements.strengthLabel.textContent = "Network load";
  elements.variabilityLabel.textContent = "Throughput";
  elements.noveltyLabel.textContent = "Load change";
  elements.embodimentSubtitle.textContent = "Joint amplitude scales directly with measured load";
  document.querySelector('[data-metric="strength"] .metric-chart').setAttribute("aria-label", "Normalized network load over recent live samples");
  document.querySelector('[data-metric="variability"] .metric-chart').setAttribute("aria-label", "Normalized throughput over recent live samples");
  document.querySelector('[data-metric="novelty"] .metric-chart').setAttribute("aria-label", "Network load change over recent live samples");
  elements.traceHashValue.textContent = "live capture";
  elements.languageValue.textContent = "direct mapping";
  elements.divergenceValue.textContent = "visible graph";
}

async function pollLive() {
  try {
    const useHistory = freezeLiveFrame && ["min", "max"].includes(evidenceSelection);
    const response = await fetch(useHistory ? "/api/trace" : "/api/state", { cache: "no-store" });
    if (!response.ok) throw new Error(`Live state request failed (${response.status})`);
    const payload = await response.json();
    const validHistory = Array.isArray(payload) ? payload.filter((entry) => entry.valid) : [];
    const sample = useHistory
      ? validHistory.reduce((selected, entry) => {
        if (!selected) return entry;
        return evidenceSelection === "max"
          ? (entry.load > selected.load ? entry : selected)
          : (entry.load < selected.load ? entry : selected);
      }, null)
      : payload;
    if (!sample) throw new Error("No valid measured frame is available");
    if (!sample.valid) throw new Error(sample.error || "Network counter is invalid");
    if (sample.sequence !== state.lastLiveSequence) {
      state.lastLiveSequence = sample.sequence;
      state.frames.push(frameFromLiveSample(sample));
      if (state.frames.length > 180) state.frames.shift();
      state.equalizerSamples = useHistory ? validHistory : state.frames;
      renderCharts(state.frames);
      render(state.frames.length - 1);
    }
    elements.shell.classList.remove("connection-lost");
  } catch (error) {
    elements.shell.dataset.state = "STALE";
    elements.shell.classList.add("connection-lost");
    elements.causeText.textContent = error instanceof Error ? error.message : String(error);
    elements.freshnessValue.textContent = "STALE";
  } finally {
    if (!freezeLiveFrame) window.setTimeout(pollLive, 250);
  }
}

function tick(now) {
  if (state.playing && now - state.lastTick >= frameDuration) {
    const steps = Math.max(1, Math.floor((now - state.lastTick) / frameDuration));
    const next = state.sequence + steps;
    if (next >= state.frames.length) {
      render(state.frames.length - 1);
      setPlaying(false);
    } else {
      render(next);
      state.lastTick += steps * frameDuration;
    }
  }
  requestAnimationFrame(tick);
}

function sequenceFromPointer(event) {
  const rect = elements.timeline.getBoundingClientRect();
  return clampSequence(((event.clientX - rect.left) / rect.width) * (state.frames.length - 1));
}

function bindInteractions() {
  elements.replayButton.addEventListener("click", () => { render(0); setPlaying(true); });
  elements.pauseButton.addEventListener("click", () => setPlaying(!state.playing));
  elements.resetButton.addEventListener("click", () => { setPlaying(false); render(0); });
  elements.senderStopButton.addEventListener("click", () => { setPlaying(false); render(540); });
  for (const band of document.querySelectorAll(".state-band")) {
    band.addEventListener("click", () => { setPlaying(false); render(Number(band.dataset.sequence)); });
  }
  elements.timeline.addEventListener("pointerdown", (event) => {
    if (event.target.closest(".state-band")) return;
    setPlaying(false);
    render(sequenceFromPointer(event));
  });
  window.addEventListener("keydown", (event) => {
    if (event.code === "Space") { event.preventDefault(); setPlaying(!state.playing); }
    if (event.code === "ArrowLeft") { event.preventDefault(); setPlaying(false); render(state.sequence - 1); }
    if (event.code === "ArrowRight") { event.preventDefault(); setPlaying(false); render(state.sequence + 1); }
  });
}

async function start() {
  try {
    if (liveMode) {
      configureLiveView();
      await pollLive();
      return;
    }
    const evidence = await loadEvidence();
    state.frames = evidence.frames;
    state.verification = evidence.verification;
    renderCharts(state.frames);
    elements.traceHashValue.textContent = evidence.verification.sha256.slice(0, 8);
    elements.languageValue.textContent = evidence.verification.candidates.includes("rust") ? "C ≡ Rust" : "reference only";
    elements.divergenceValue.textContent = evidence.verification.first_divergence ? "detected" : "none";
    bindInteractions();
    render(365);
    requestAnimationFrame(tick);
  } catch (error) {
    elements.errorMessage.textContent = error instanceof Error ? error.message : String(error);
    elements.errorScreen.hidden = false;
  }
}

start();
