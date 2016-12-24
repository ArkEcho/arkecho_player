using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
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
        
        public delegate void MainActivityModelDelegate(String message);
        public event MainActivityModelDelegate newMessageReceived;

        public void emitNewMessageReceived(String message)
        {
            // Prüft ob das Event überhaupt einen Abonnenten hat.
            if (newMessageReceived != null) newMessageReceived(message);
        }

        public async void connectWebSocket(String address)
        {
            failed_ = false;
            timeOut();

            webSocket_.Open(address);

            while (!webSocket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        public void sendMessage(String message)
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