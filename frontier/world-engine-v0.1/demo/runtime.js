const contracts = {
  wlan: {
    title: "Connected WLAN link",
    tasks: "Read link quality · timestamp the sample · record validity and freshness.",
    source: "Windows connected-link adapter",
    boundary: "The adapter reads a property of the host’s active link. It does not observe the whole room or identify people.",
  },
  causal: {
    title: "Causal frame",
    tasks: "Carry sequence · timestamp · source class · validity · bounded input.",
    source: "Sensor adapter output",
    boundary: "The frame preserves provenance. It is an observation record, not the world itself.",
  },
  features: {
    title: "Fixed-point features",
    tasks: "Derive bounded strength · variability · novelty with integer operations.",
    source: "Causal frame (immutable input)",
    boundary: "Features are deterministic statistics. They are not pose, occupancy, emotion, or physiology.",
  },
  membrane: {
    title: "State membrane",
    tasks: "Apply thresholds · hysteresis · dwell · immediate stale failure.",
    source: "Feature frame + fixed configuration",
    boundary: "CALM, ATTUNE, STORM, and STALE are project states, not physical diagnoses.",
  },
  pose: {
    title: "Pose + hash",
    tasks: "Emit bounded integer pose · serialize · hash · replay.",
    source: "Math-only authority runtime",
    boundary: "This is the only contract consumers need. Consumers cannot mutate or re-author measurement truth.",
  },
  ui: {
    title: "UI — see and inspect",
    tasks: "Scrub frames · click nodes · compare states · expose provenance.",
    source: "Pose + hash (immutable)",
    boundary: "The UI explains and displays. It does not reclassify the signal.",
  },
  game: {
    title: "Game — play and score",
    tasks: "Map state · trigger mechanics · reward verified events.",
    source: "Pose + hash (immutable)",
    boundary: "Game logic may interpret verified states, but cannot rewrite measurement truth.",
    examples: [
      ["ATTUNE → reveal path", "Reward exploration.", "attune"],
      ["STORM → charge hazard", "Create a challenge.", "storm"],
      ["STALE → pause scoring", "Do not reward missing data.", "stale"],
    ],
  },
  renderer: {
    title: "Renderer — embody motion",
    tasks: "Resolve bones · convert integer units · apply pose only.",
    source: "Pose + hash (immutable)",
    boundary: "Lens, browser, and game-engine renderers may differ visually while preserving the same authoritative pose frame.",
  },
  ai: {
    title: "AI assistant — explain and author",
    tasks: "Explain evidence · compare traces · propose mappings and configurations for human review.",
    source: "Read-only evidence + explicit authoring task",
    boundary: "AI is never authoritative in the fast loop. Any accepted configuration change creates a new versioned evidence run.",
  },
};

const title = document.getElementById("selectedTitle");
const tasks = document.getElementById("selectedTasks");
const source = document.getElementById("selectedSource");
const boundary = document.getElementById("selectedBoundary");
const examples = document.getElementById("mappingExamples");
const exampleList = document.getElementById("exampleList");

function selectNode(key) {
  const contract = contracts[key];
  if (!contract) return;
  document.querySelectorAll(".runtime-node").forEach((node) => {
    const selected = node.dataset.node === key;
    node.classList.toggle("selected", selected);
    node.setAttribute("aria-pressed", String(selected));
  });
  title.textContent = contract.title;
  tasks.textContent = contract.tasks;
  source.textContent = contract.source;
  boundary.textContent = contract.boundary;
  examples.hidden = !contract.examples;
  if (contract.examples) {
    exampleList.replaceChildren(...contract.examples.map(([heading, detail, className]) => {
      const item = document.createElement("li");
      item.className = className;
      const strong = document.createElement("strong");
      const span = document.createElement("span");
      strong.textContent = heading;
      span.textContent = detail;
      item.append(strong, span);
      return item;
    }));
  }
}

document.querySelectorAll(".runtime-node").forEach((node) => {
  node.setAttribute("aria-pressed", String(node.dataset.node === "game"));
  node.addEventListener("click", () => selectNode(node.dataset.node));
});
