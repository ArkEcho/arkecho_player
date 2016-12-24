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

        public async void connectWebSocket(String address)
        {
            failed_ = false;
            startTimeOut();

            webSocket_.Open(address);

            while (!webSocket_.IsOpen && !failed_)
            {
                await Task.Delay(10);
            }
        }

        private async void startTimeOut()
        {
            await Task.Delay(2000);
            failed_ = true;
        }

        private void onWebSocketOpened()
        {
        }

        private void onWebSocketMessage(string obj)
        {
        }

        private void onWebSocketError(string obj)
        {
            failed_ = true;
        }

        private void onWebSocketLog(string obj)
        {
        }
    }
}