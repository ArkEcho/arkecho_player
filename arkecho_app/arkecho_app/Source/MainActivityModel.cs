using System.Threading.Tasks;

namespace arkecho_app
{
    public class MainActivityModel
    {
        private Websockets.IWebSocketConnection webSocket_;
        private bool failed_;

        public MainActivityModel()
        {
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
        
        public delegate void MainActivityModelDelegate(string message);
        public event MainActivityModelDelegate newMessageReceived;

        public void emitNewMessageReceived(string message)
        {
            // Pr�ft ob das Event �berhaupt einen Abonnenten hat.
            if (newMessageReceived != null) newMessageReceived(message);
        }

        public async void connectWebSocket(string address)
        {
            failed_ = false;
            timeOut();

            webSocket_.Open(address);

            while (!webSocket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        public void sendMessage(string message)
        {
            if (!webSocket_.IsOpen) return;
            webSocket_.Send(message);
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