using System.Threading.Tasks;

namespace arkecho_app
{
    public class MainActivityModel
    {
        private Websockets.IWebSocketConnection webSocket_;
        private bool failed_;
        public delegate void MainActivityModelDelegate(string message);
        public event MainActivityModelDelegate newMessageReceived;
        
        public MainActivityModel()
        {
            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();

            webSocket_ = Websockets.WebSocketFactory.Create();
            webSocket_.OnOpened += onWebSocketOpened;
            webSocket_.OnMessage += onWebSocketMessage;
            webSocket_.OnError += onWebSocketError;
            webSocket_.OnLog += onWebSocketLog;
        }

        ~MainActivityModel()
        {
            if(webSocket_.IsOpen) webSocket_.Close();
        }
        
        public void emitNewMessageReceived(string message)
        {
            // Prüft ob das Event überhaupt einen Abonnenten hat.
            if (newMessageReceived != null) newMessageReceived(message);
        }

        public async Task connectWebSocket(string address)
        {
            failed_ = false;
            timeOut();

            webSocket_.Open(address);

            while (!webSocket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        public void sendMessage(int messageType, string message)
        {
            if (!webSocket_.IsOpen) return;
            webSocket_.Send(MessageHandler.createMessage(messageType, message));
        }

        private async void timeOut()
        {
            await Task.Delay(2000);
            failed_ = true;
        }

        private void onWebSocketOpened()
        {
        }

        private void onWebSocketMessage(string message)
        {
            int typ = MessageHandler.handleReceivedMessage(ref message);
            emitNewMessageReceived(message);
        }

        private void onWebSocketError(string error)
        {
            failed_ = true;
        }

        private void onWebSocketLog(string obj)
        {
        }
    }
}