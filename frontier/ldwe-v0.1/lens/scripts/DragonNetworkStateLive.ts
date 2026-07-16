@component
export class DragonNetworkStateLive extends BaseScriptComponent {
  // Written by the LEGION Pose Bridge. No authoritative wave generation occurs here.
  @input sourceCode: number = 3;
  @input liveSequence: number = 0;
  @input liveFreshnessMs: number = 999999;
  @input behaviorCode: number = 3;
  @input frameHashPrefix: string = "00000000";

  @input rootYUm: number = 0;
  @input rootRollMdeg: number = 0;
  @input neck1PitchMdeg: number = 0;
  @input neck2PitchMdeg: number = 0;
  @input headYawMdeg: number = 0;
  @input wingLRollMdeg: number = 0;
  @input wingRRollMdeg: number = 0;
  @input tail1YawMdeg: number = 0;
  @input tail2YawMdeg: number = 0;
  @input tail3YawMdeg: number = 0;

  @input rootUnitScale: number = 0.000001;
  @input printDiagnostics: boolean = true;

  private root: SceneObject;
  private rootTransform: Transform;
  private rootBasePosition: vec3;
  private rootBaseRotation: quat;

  private neck1: SceneObject | null = null;
  private neck2: SceneObject | null = null;
  private head: SceneObject | null = null;
  private wingL: SceneObject | null = null;
  private wingR: SceneObject | null = null;
  private tail1: SceneObject | null = null;
  private tail2: SceneObject | null = null;
  private tail3: SceneObject | null = null;

  private neck1Base: quat | null = null;
  private neck2Base: quat | null = null;
  private headBase: quat | null = null;
  private wingLBase: quat | null = null;
  private wingRBase: quat | null = null;
  private tail1Base: quat | null = null;
  private tail2Base: quat | null = null;
  private tail3Base: quat | null = null;
  private lastPrintedSecond: number = -1;

  onAwake(): void {
    this.root = this.getSceneObject();
    this.rootTransform = this.root.getTransform();
    this.rootBasePosition = this.rootTransform.getLocalPosition();
    this.rootBaseRotation = this.rootTransform.getLocalRotation();

    this.neck1 = this.findFirst(["ACT_neck_1"]);
    this.neck2 = this.findFirst(["ACT_neck_2", "ACT_neck_3"]);
    this.head = this.findFirst(["head"]);
    this.wingL = this.findFirst(["w_C_L", "upper_arm_L"]);
    this.wingR = this.findFirst(["w_C_R", "upper_arm_R"]);
    this.tail1 = this.findFirst(["tail_1"]);
    this.tail2 = this.findFirst(["tail_2"]);
    this.tail3 = this.findFirst(["tail_3"]);

    this.neck1Base = this.capture(this.neck1);
    this.neck2Base = this.capture(this.neck2);
    this.headBase = this.capture(this.head);
    this.wingLBase = this.capture(this.wingL);
    this.wingRBase = this.capture(this.wingR);
    this.tail1Base = this.capture(this.tail1);
    this.tail2Base = this.capture(this.tail2);
    this.tail3Base = this.capture(this.tail3);

    if (this.printDiagnostics) {
      print("[LEGION PoseDriver] AWAKE");
      print("[LEGION PoseDriver] neck1=" + this.nameOf(this.neck1));
      print("[LEGION PoseDriver] neck2=" + this.nameOf(this.neck2));
      print("[LEGION PoseDriver] head=" + this.nameOf(this.head));
      print("[LEGION PoseDriver] wingL=" + this.nameOf(this.wingL));
      print("[LEGION PoseDriver] wingR=" + this.nameOf(this.wingR));
      print("[LEGION PoseDriver] tail1=" + this.nameOf(this.tail1));
      print("[LEGION PoseDriver] tail2=" + this.nameOf(this.tail2));
      print("[LEGION PoseDriver] tail3=" + this.nameOf(this.tail3));
    }

    this.createEvent("UpdateEvent").bind(() => this.applyPose());
  }

  private findFirst(names: string[]): SceneObject | null {
    for (let i = 0; i < names.length; i++) {
      const found = this.findRecursive(this.root, names[i]);
      if (found !== null) return found;
    }
    return null;
  }

  private findRecursive(current: SceneObject, target: string): SceneObject | null {
    if (current.name === target) return current;
    for (let i = 0; i < current.getChildrenCount(); i++) {
      const found = this.findRecursive(current.getChild(i), target);
      if (found !== null) return found;
    }
    return null;
  }

  private capture(object: SceneObject | null): quat | null {
    return object === null ? null : object.getTransform().getLocalRotation();
  }

  private nameOf(object: SceneObject | null): string {
    return object === null ? "NOT_FOUND" : object.name;
  }

  private radians(millidegrees: number): number {
    return millidegrees * Math.PI / 180000.0;
  }

  private applyAxis(object: SceneObject | null, base: quat | null, angleMdeg: number, axis: vec3): void {
    if (object === null || base === null) return;
    object.getTransform().setLocalRotation(base.multiply(quat.angleAxis(this.radians(angleMdeg), axis)));
  }

  private behaviorName(): string {
    if (this.behaviorCode === 0) return "CALM";
    if (this.behaviorCode === 1) return "ATTUNE";
    if (this.behaviorCode === 2) return "STORM";
    return "STALE";
  }

  private sourceName(): string {
    if (this.sourceCode === 0) return "SIMULATED";
    if (this.sourceCode === 1) return "RECORDED_REPLAY";
    if (this.sourceCode === 2) return "LIVE_WLAN";
    return "STALE_WLAN";
  }

  private applyPose(): void {
    this.rootTransform.setLocalPosition(new vec3(
      this.rootBasePosition.x,
      this.rootBasePosition.y + this.rootYUm * this.rootUnitScale,
      this.rootBasePosition.z
    ));
    this.rootTransform.setLocalRotation(this.rootBaseRotation.multiply(
      quat.angleAxis(this.radians(this.rootRollMdeg), new vec3(0, 0, 1))
    ));

    this.applyAxis(this.neck1, this.neck1Base, this.neck1PitchMdeg, new vec3(1, 0, 0));
    this.applyAxis(this.neck2, this.neck2Base, this.neck2PitchMdeg, new vec3(1, 0, 0));
    this.applyAxis(this.head, this.headBase, this.headYawMdeg, new vec3(0, 1, 0));
    this.applyAxis(this.wingL, this.wingLBase, this.wingLRollMdeg, new vec3(0, 0, 1));
    this.applyAxis(this.wingR, this.wingRBase, this.wingRRollMdeg, new vec3(0, 0, 1));
    this.applyAxis(this.tail1, this.tail1Base, this.tail1YawMdeg, new vec3(0, 1, 0));
    this.applyAxis(this.tail2, this.tail2Base, this.tail2YawMdeg, new vec3(0, 1, 0));
    this.applyAxis(this.tail3, this.tail3Base, this.tail3YawMdeg, new vec3(0, 1, 0));

    const second = Math.floor(getTime());
    if (this.printDiagnostics && second !== this.lastPrintedSecond) {
      this.lastPrintedSecond = second;
      print(`[LEGION PoseDriver] source=${this.sourceName()} state=${this.behaviorName()} seq=${this.liveSequence.toFixed(0)} fresh=${this.liveFreshnessMs.toFixed(0)}ms hash=${this.frameHashPrefix}`);
    }
  }
}
