using System.Threading.Tasks;

namespace arkecho_app
{
    public class ArkEchoWebSocket
    {
        private Websockets.IWebSocketConnection socket_;
        private bool failed_;

        public delegate void WebSocketDelegateMessage(string message);
        public event WebSocketDelegateMessage newMessageReceived;

        public delegate void WebSocketDelegateClosed();
        public event WebSocketDelegateClosed webSocketConnectionClosed;

        public ArkEchoWebSocket()
        {
            socket_ = Websockets.WebSocketFactory.Create();
            socket_.OnMessage += onWebSocketMessage;
            socket_.OnError += onWebSocketError;
            socket_.OnClosed += onWebSocketClosed;
            //socket_.OnOpened += onWebSocketOpened;
            //socket_.OnLog += onWebSocketLog;
        }

        private void emitNewMessageReceived(string message)
        {
            if (newMessageReceived != null) newMessageReceived(message);
        }

        private void emitWebSocketConnectionClosed()
        {
            if (webSocketConnectionClosed != null) webSocketConnectionClosed();
        }

        public async Task connectWebSocket(string address)
        {
            failed_ = false;
            timeOut();

            socket_.Open(address);

            while (!socket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        public void sendMessage(int messageType, string message)
        {
            if (!socket_.IsOpen) return;
            socket_.Send(MessageHandler.createMessage(messageType, message));
        }

        public bool connectionIsOpen()
        {
            return socket_.IsOpen;
        }

        private async void timeOut()
        {
            await Task.Delay(2000);
            failed_ = true;
        }

        private void onWebSocketMessage(string message)
        {
            //int typ = MessageHandler.handleReceivedMessage(ref message);
            emitNewMessageReceived(message);
        }

        private void onWebSocketError(string error)
        {
            failed_ = true;
        }

        private void onWebSocketClosed()
        {
            emitWebSocketConnectionClosed();
        }
    }
}