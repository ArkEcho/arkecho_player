using Android.App;
using Android.OS;
using System.Threading.Tasks;
using System;

namespace arkecho_app.source
{
    [Activity(Label = "@string/ApplicationTitle")]
    public class PlayerActivity : Activity
    {
        bool backPressed = false;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.PlayerActivity);

            ArkEchoWebSocket.webSocketConnectionClosed += onWebSocketConnectionClosed;
        }

        private void onWebSocketConnectionClosed()
        {
            if (!backPressed)
            {
                AlertDialog.Builder alert = new AlertDialog.Builder(this);
                alert.SetTitle(Resource.String.MsgBoxTitleError);
                alert.SetMessage(Resource.String.MsgBoxTextConnectionEnded);
                alert.SetPositiveButton("Ok", (senderAlert, args) => { Finish(); });
                RunOnUiThread(() => alert.Show());
            }
        }

        public override async void OnBackPressed()
        {
            backPressed = true;
            ArkEchoWebSocket.disconnectWebSocket();
            await Task.Delay(10);
            Finish();
            //RunOnUiThread(() => base.OnBackPressed());
        }
    }
}