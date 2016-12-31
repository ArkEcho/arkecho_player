using Android.App;
using Android.OS;
using Android.Widget;

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

            FindViewById<Button>(Resource.Id.pbPlay_Pause).Click += onPbPlay_PauseClicked;
            FindViewById<Button>(Resource.Id.pbForward).Click += onPbForwardClicked;
            FindViewById<Button>(Resource.Id.pbBackward).Click += onPbBackwardClicked;

            ArkEchoWebSocket.webSocketConnectionClosed += onWebSocketConnectionClosed;
        }

        private void onPbBackwardClicked(object sender, EventArgs e)
        {
            ArkEchoWebSocket.sendMessage((int)MessageHandler.MESSAGETYPE.MT_BACKWARD, "");
        }

        private void onPbForwardClicked(object sender, EventArgs e)
        {
            ArkEchoWebSocket.sendMessage((int)MessageHandler.MESSAGETYPE.MT_FORWARD, "");
        }

        private void onPbPlay_PauseClicked(object sender, EventArgs e)
        {
            ArkEchoWebSocket.sendMessage((int)MessageHandler.MESSAGETYPE.MT_PLAY_PAUSE, "");
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