using Android.App;
using Android.Widget;
using Android.OS;
using System.Net.WebSockets;

namespace arkecho_app
{
    [Activity(Label = "ArkEcho", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            // Set our view from the "main" layout resource
            SetContentView (Resource.Layout.Main);

            FindViewById<Button>(Resource.Id.pbClickMe).Click += onPbClickMeClick;
        }

        private void onPbClickMeClick(object sender, System.EventArgs e)
        {
            ClientWebSocket socket = new ClientWebSocket();
            System.Threading.CancellationToken token;
            System.Uri uri = new System.Uri("ws://192.168.178.20:1000");

            socket.ConnectAsync(uri, token);
            //TextView lblHelloWorld = FindViewById<TextView>(Resource.Id.lblHelloWorld);
            //lblHelloWorld.Text = "Test";
        }
    }
}

