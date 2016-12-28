using System.Threading.Tasks;

namespace arkecho_app
{
    public static class ArkEchoWebSocket
    {
        private static Websockets.IWebSocketConnection socket_;
        private static bool failed_;

        public delegate void WebSocketDelegateMessage(string message);
        public static event WebSocketDelegateMessage newMessageReceived;

        public delegate void WebSocketDelegateClosed();
        public static event WebSocketDelegateClosed webSocketConnectionClosed;

        static ArkEchoWebSocket()
        {
            socket_ = Websockets.WebSocketFactory.Create();
            socket_.OnMessage += onWebSocketMessage;
            socket_.OnError += onWebSocketError;
            socket_.OnClosed += onWebSocketClosed;
            //socket_.OnOpened += onWebSocketOpened;
            //socket_.OnLog += onWebSocketLog;
        }

        private static void emitNewMessageReceived(string message)
        {
            if (newMessageReceived != null) newMessageReceived(message);
        }

        private static void emitWebSocketConnectionClosed()
        {
            if (webSocketConnectionClosed != null) webSocketConnectionClosed();
        }

        public static async Task connectWebSocket(string address)
        {
            failed_ = false;
            timeOut();

            socket_.Open(address);

            while (!socket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        public static void sendMessage(int messageType, string message)
        {
            if (!socket_.IsOpen) return;
            socket_.Send(MessageHandler.createMessage(messageType, message));
        }

        public static bool connectionIsOpen()
        {
            return socket_.IsOpen;
        }

        public static void disconnectWebSocket()
        {
            socket_.Close();
        }

        private static async void timeOut()
        {
            await Task.Delay(2000);
            failed_ = true;
        }

        private static void onWebSocketMessage(string message)
        {
            //int typ = MessageHandler.handleReceivedMessage(ref message);
            emitNewMessageReceived(message);
        }

        private static void onWebSocketError(string error)
        {
            failed_ = true;
        }

        private static void onWebSocketClosed()
        {
            disconnectWebSocket();
            emitWebSocketConnectionClosed();
        }
    }
}