const byId = (id) => document.getElementById(id);

const parts = {
  root: byId("dragonRoot"),
  neck1: byId("neck1"),
  neck2: byId("neck2"),
  head: byId("head"),
  wingL: byId("wingL"),
  wingR: byId("wingR"),
  tail1: byId("tail1"),
  tail2: byId("tail2"),
  tail3: byId("tail3"),
};

function rotate(element, degrees, x, y) {
  element.style.transformOrigin = `${x}px ${y}px`;
  element.style.transform = `rotate(${degrees}deg)`;
}

export function applyDragonPose(frame) {
  const pose = frame.pose;
  const rootY = Math.max(-18, Math.min(18, pose.root_y_um / 4200));
  const rootRoll = pose.root_roll_mdeg / 1000;
  parts.root.style.transformOrigin = "370px 390px";
  parts.root.style.transform = `translateY(${-rootY}px) rotate(${rootRoll}deg)`;

  rotate(parts.neck1, pose.neck1_pitch_mdeg / 1000, 309, 340);
  rotate(parts.neck2, pose.neck2_pitch_mdeg / 1000, 276, 220);
  rotate(parts.head, pose.head_yaw_mdeg / 1000, 149, 123);
  rotate(parts.wingL, pose.wing_l_roll_mdeg / 1000, 331, 351);
  rotate(parts.wingR, pose.wing_r_roll_mdeg / 1000, 409, 351);
  rotate(parts.tail1, pose.tail1_yaw_mdeg / 1000, 469, 413);
  rotate(parts.tail2, pose.tail2_yaw_mdeg / 1000, 615, 476);
  rotate(parts.tail3, pose.tail3_yaw_mdeg / 1000, 710, 523);
}
