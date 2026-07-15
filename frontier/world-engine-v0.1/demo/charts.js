const WIDTH = 320;
const HEIGHT = 96;

function pointsFor(frames, key) {
  const last = Math.max(frames.length - 1, 1);
  return frames.map((frame, index) => {
    const x = (index / last) * WIDTH;
    const value = frame.features_q16[key] / 65_535;
    const y = HEIGHT - value * HEIGHT;
    return `${x.toFixed(2)},${y.toFixed(2)}`;
  }).join(" ");
}

export function renderCharts(frames) {
  for (const key of ["strength", "variability", "novelty"]) {
    const line = document.querySelector(`[data-chart="${key}"] .chart-line`);
    line.setAttribute("points", pointsFor(frames, key));
  }
}
