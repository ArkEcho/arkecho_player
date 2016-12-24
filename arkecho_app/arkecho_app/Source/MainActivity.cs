using Android.App;
using Android.Widget;
using Android.OS;
using System.Threading.Tasks;

namespace arkecho_app
{
    [Activity(Label = "ArkEcho", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        private Websockets.IWebSocketConnection connection;
        private bool Failed;
        private bool Echo;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            // Set our view from the "main" layout resource
            SetContentView (Resource.Layout.Main);
            FindViewById<Button>(Resource.Id.pbClickMe).Click += onPbClickMeClick;
            
            Websockets.Droid.WebsocketConnection.Link();
        }

        private async void onPbClickMeClick(object sender, System.EventArgs e)
        {
            connection = Websockets.WebSocketFactory.Create();
            connection.OnOpened += Connection_OnOpened;
            connection.OnMessage += Connection_OnMessage;
            connection.OnError += Connection_OnError;
            connection.OnLog += Connection_OnLog;
            
            Echo = Failed = false;
            Timeout();
            
            connection.Open("ws://192.168.178.20:1000");

            while (!connection.IsOpen && !Failed)
            {
                await Task.Delay(10);
            }

            if (!connection.IsOpen) return;
            
            connection.Send("Hello World");

            while (!Echo && !Failed)
            {
                await Task.Delay(10);
            }

            if (!Echo) return;
        }

        async void Timeout()
        {
            await Task.Delay(120000);
            Failed = true;
        }

        private void Connection_OnOpened()
        {
        }

        private void Connection_OnMessage(string obj)
        {
        }

        private void Connection_OnError(string obj)
        {
            Failed = true;
        }

        private void Connection_OnLog(string obj)
        {
        }
    }
}

