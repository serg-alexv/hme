const Q16_MAX = 65_535;

export async function loadEvidence() {
  const [traceResponse, verificationResponse] = await Promise.all([
    fetch("data/golden_pose.jsonl", { cache: "no-store" }),
    fetch("data/verification.json", { cache: "no-store" }),
  ]);
  if (!traceResponse.ok) throw new Error(`Trace request failed (${traceResponse.status})`);
  if (!verificationResponse.ok) throw new Error(`Verification request failed (${verificationResponse.status})`);

  const traceText = await traceResponse.text();
  const frames = traceText.trimEnd().split("\n").map((line, index) => {
    const frame = JSON.parse(line);
    if (frame.schema !== "world.pose/0.1" || frame.sequence !== index) {
      throw new Error(`Trace contract failed at line ${index + 1}`);
    }
    return frame;
  });
  const verification = await verificationResponse.json();
  if (verification.status !== "PASS" || verification.frames !== frames.length) {
    throw new Error("Committed verification report does not cover this trace");
  }
  return { frames, verification };
}

export function percent(q16) {
  return Math.round((Number(q16) / Q16_MAX) * 100);
}

export function degrees(millidegrees) {
  return Number(millidegrees) / 1000;
}

export function signedDegrees(millidegrees) {
  const value = degrees(millidegrees);
  return `${value >= 0 ? "+" : "−"}${Math.abs(value).toFixed(1)}°`;
}

export function causeFor(frame) {
  switch (frame.behavior) {
    case "STORM":
      return "Variability and novelty stayed above the STORM gate for 6 frames.";
    case "ATTUNE":
      return "Variability or novelty stayed above the ATTUNE gate for 8 frames.";
    case "STALE":
      return "The input became invalid and freshness exceeded the 1,500 ms safety gate.";
    default:
      return "Variability and novelty remained below the ATTUNE gate.";
  }
}

export function sourceLabel(source) {
  return {
    SIMULATED: "Verified synthetic replay",
    RECORDED_REPLAY: "Recorded WLAN replay",
    LIVE_WLAN: "Live connected-WLAN telemetry",
    WINDOWS_INTERFACE_BYTES: "Live Windows network byte counters",
    STALE_WLAN: "Sender stopped / stale input",
  }[source] ?? "Rejected unknown source";
}
