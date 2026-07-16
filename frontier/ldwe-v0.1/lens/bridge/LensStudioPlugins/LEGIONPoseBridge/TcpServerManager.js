import * as Network from "LensStudio:Network";

export default class TcpServerManager {
    constructor() {
        this.server = Network.TcpServer.create();
        this.connections = [];
        this.sockets = [];
        this.onClientConnected = null;
        this.onClientDataReceived = null;
        this.onClientDisconnected = null;
        this.onClientSocketError = null;
        this.enableLogging = false;

        this.connections.push(this.server.onConnect.connect((socket) => {
            this.sockets.push(socket);
            if (this.enableLogging) {
                console.log(`[LEGION Bridge] client ${socket.remoteAddress.address}:${socket.remoteAddress.port}`);
            }
            if (this.onClientConnected) this.onClientConnected(socket);
            this.connections.push(socket.onData.connect((data) => {
                if (this.onClientDataReceived) this.onClientDataReceived(data, socket);
            }));
            this.connections.push(socket.onEnd.connect(() => {
                if (this.onClientDisconnected) this.onClientDisconnected(socket);
            }));
            this.connections.push(socket.onError.connect((error) => {
                if (this.onClientSocketError) this.onClientSocketError(error, socket);
            }));
        }));
    }

    start(address, port) {
        const addr = new Network.Address();
        addr.address = address;
        addr.port = port;
        this.server.listen(addr);
        console.log(`[LEGION Bridge] TCP listening on ${address}:${port}`);
    }

    close() {
        this.connections.forEach((connection) => {
            try { connection.disconnect(); } catch (_) {}
        });
        this.connections = [];
        try { this.server.close(); } catch (_) {}
    }
}
