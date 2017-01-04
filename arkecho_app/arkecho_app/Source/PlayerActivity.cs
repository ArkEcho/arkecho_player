using Android.App;
using Android.OS;
using Android.Widget;

using System.Threading.Tasks;
using System;
using Newtonsoft.Json.Linq;
using Android.Graphics.Drawables;

namespace arkecho_app.source
{
    [Activity(Label = "@string/ApplicationTitle")]
    public class PlayerActivity : Activity
    {
        private const string JSON_COVER_ART = "CoverArt";
        private const string JSON_SONG_TITLE = "SongTitle";
        private const string JSON_SONG_INTERPRET = "SongInterpret";
        private const string JSON_ALBUM_TITLE = "AlbumTitle";
        private const string JSON_ALBUM_INTERPRET = "AlbumInterpret";

        bool backPressed = false;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.PlayerActivity);

            FindViewById<Button>(Resource.Id.pbPlay_Pause).Click += onPbPlay_PauseClicked;
            FindViewById<Button>(Resource.Id.pbForward).Click += onPbForwardClicked;
            FindViewById<Button>(Resource.Id.pbBackward).Click += onPbBackwardClicked;

            ArkEchoWebSocket.newMessageReceived += onWebSocketNewMessageReceived;
            ArkEchoWebSocket.webSocketConnectionClosed += onWebSocketConnectionClosed;
        }

        private void onWebSocketNewMessageReceived(string message)
        {
            int messageType = MessageHandler.handleReceivedMessage(ref message);
            if(messageType == (int)MessageHandler.MESSAGETYPE.MT_SONG_ACTUAL)
            {
                JObject obj = JObject.Parse(message);
                string songTitle = obj[JSON_SONG_TITLE].ToObject<string>();
                string songInterpret = obj[JSON_SONG_INTERPRET].ToObject<string>();
                //string albumTitle = obj[JSON_ALBUM_TITLE].ToObject<string>();
                //string albumInterpret = obj[JSON_ALBUM_INTERPRET].ToObject<string>();
                //string coverArtBase64 = obj[JSON_COVER_ART].ToObject<string>();
                //Drawable coverArtImage = MessageHandler.base64ToImage(coverArtBase64);
                //ImageView iv = FindViewById<ImageView>(Resource.Id.imageViewCoverArt);
                //iv.SetImageDrawable(coverArtImage);
                TextView title = FindViewById<TextView>(Resource.Id.tvTitlePlayer);
                RunOnUiThread(() => title.Text = songTitle);

                TextView inter = FindViewById<TextView>(Resource.Id.tvInterpretPlayer);
                RunOnUiThread(() => inter.Text = songInterpret);

            }
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