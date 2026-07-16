import CoreService from "LensStudio:CoreService";
import TcpServerManager from "./TcpServerManager.js";

const HOST = "127.0.0.1";
const PORT = 19331;
const TARGET_SCRIPT_NAME = "DragonNetworkStateLive";

export class LegionPoseBridge extends CoreService {
    static descriptor() {
        return {
            id: "com.world20.legion.posebridge",
            interfaces: CoreService.descriptor().interfaces,
            name: "LEGION Pose Bridge",
            description: "Applies deterministic legion.pose/0.1 frames to the dragon.",
            dependencies: [Editor.Model.IModel]
        };
    }

    constructor(pluginSystem) {
        super(pluginSystem);
        this.model = null;
        this.server = null;
        this.buffer = "";
        this.target = null;
        this.lastSequence = -1;
        this.appliedFrames = 0;
    }

    start() {
        this.model = this.pluginSystem.findInterface(Editor.Model.IModel);
        this.server = new TcpServerManager();
        this.server.enableLogging = true;
        this.server.onClientConnected = () => {
            console.log("[LEGION Bridge] client connected");
            this.target = null;
        };
        this.server.onClientDataReceived = (data) => {
            this.receiveData(typeof data === "string" ? data : data.toString());
        };
        this.server.onClientDisconnected = () => {
            console.log("[LEGION Bridge] client disconnected; forcing STALE");
            this.applyStale();
        };
        this.server.onClientSocketError = (error) => {
            console.error("[LEGION Bridge] socket error: " + error);
            this.applyStale();
        };
        this.server.start(HOST, PORT);
    }

    stop() {
        this.applyStale();
        if (this.server) this.server.close();
        this.server = null;
    }

    receiveData(text) {
        this.buffer += text;
        while (true) {
            const newline = this.buffer.indexOf("\n");
            if (newline < 0) return;
            const line = this.buffer.slice(0, newline).trim();
            this.buffer = this.buffer.slice(newline + 1);
            if (!line) continue;
            try {
                this.applyFrame(JSON.parse(line));
            } catch (error) {
                console.error("[LEGION Bridge] invalid JSON: " + error);
            }
        }
    }

    resolveTarget() {
        if (this.target) return this.target;
        const components = this.model.project.scene.findComponents("ScriptComponent");
        for (const component of components) {
            const asset = component.scriptAsset;
            const name = asset && asset.name ? asset.name : "";
            if (name.indexOf(TARGET_SCRIPT_NAME) >= 0) {
                this.target = component;
                console.log("[LEGION Bridge] target found: " + name);
                return component;
            }
        }
        console.error("[LEGION Bridge] DragonNetworkStateLive component not found");
        return null;
    }

    integer(value, fallback = 0) {
        const n = Number(value);
        return Number.isFinite(n) ? Math.trunc(n) : fallback;
    }

    applyFrame(frame) {
        if (!frame || frame.schema !== "legion.pose/0.1") {
            console.error("[LEGION Bridge] unsupported schema");
            return;
        }
        const sequence = this.integer(frame.sequence, -1);
        if (sequence <= this.lastSequence) return;
        this.lastSequence = sequence;

        const target = this.resolveTarget();
        if (!target) return;
        const pose = frame.pose || {};
        const sourceMap = {SIMULATED: 0, RECORDED_REPLAY: 1, LIVE_WLAN: 2, STALE_WLAN: 3};
        const behaviorMap = {CALM: 0, ATTUNE: 1, STORM: 2, STALE: 3};

        target.sourceCode = sourceMap[frame.source] ?? 3;
        target.behaviorCode = behaviorMap[frame.behavior] ?? 3;
        target.liveSequence = sequence;
        target.liveFreshnessMs = this.integer(frame.freshness_ms, 999999);
        target.frameHashPrefix = String(frame.frame_hash || "00000000").slice(0, 8);
        target.rootYUm = this.integer(pose.root_y_um);
        target.rootRollMdeg = this.integer(pose.root_roll_mdeg);
        target.neck1PitchMdeg = this.integer(pose.neck1_pitch_mdeg);
        target.neck2PitchMdeg = this.integer(pose.neck2_pitch_mdeg);
        target.headYawMdeg = this.integer(pose.head_yaw_mdeg);
        target.wingLRollMdeg = this.integer(pose.wing_l_roll_mdeg);
        target.wingRRollMdeg = this.integer(pose.wing_r_roll_mdeg);
        target.tail1YawMdeg = this.integer(pose.tail1_yaw_mdeg);
        target.tail2YawMdeg = this.integer(pose.tail2_yaw_mdeg);
        target.tail3YawMdeg = this.integer(pose.tail3_yaw_mdeg);

        this.appliedFrames++;
        if (this.appliedFrames % 60 === 0) {
            console.log(`[LEGION Bridge] seq=${sequence} state=${frame.behavior} hash=${target.frameHashPrefix}`);
        }
    }

    applyStale() {
        const target = this.resolveTarget();
        if (!target) return;
        target.sourceCode = 3;
        target.behaviorCode = 3;
        target.liveFreshnessMs = 999999;
    }
}
