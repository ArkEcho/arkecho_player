using Android.App;
using Android.Widget;
using Android.OS;

using System.Threading.Tasks;
using System;
using System.Collections.Generic;

namespace arkecho_app
{
    [Activity(Label = "@string/ApplicationTitle", MainLauncher = true, Icon = "@drawable/playerIcon")]
    public class MainActivityView : Activity
    {
        MainActivityModel model_;

        List<string> items;
        ArrayAdapter<string> adapter;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            // Set our view from the "main" layout resource
            SetContentView (Resource.Layout.Main);

            // Connect Buttons
            FindViewById<Button>(Resource.Id.pbConnect).Click += onPbConnectClicked;
            FindViewById<Button>(Resource.Id.pbSendMessage).Click += onPbSendMessageClicked;

            // Prepare ListView
            items = new List<string>();
            adapter = new ArrayAdapter<string>(this, Android.Resource.Layout.SimpleListItem1, items);
            FindViewById<ListView>(Resource.Id.lvMessages).Adapter = adapter;

            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();

            // Create Model and Connect
            model_ = new MainActivityModel();
            model_.newMessageReceived += onNewMessageReceived;
        }

        private void onPbSendMessageClicked(object sender, EventArgs e)
        {
            string message = FindViewById<TextView>(Resource.Id.teMessage).Text;
            model_.sendMessage(message);
        }

        private void onNewMessageReceived(string message)
        {
            string msg = message;
            RunOnUiThread(() => adapter.Add(msg));
        }

        private void onPbConnectClicked(object sender, System.EventArgs e)
        {
            string address = FindViewById<TextView>(Resource.Id.teAddress).Text;
            model_.connectWebSocket("ws://" + address);
        }

        //ZXing.Mobile.MobileBarcodeScanner scanner = new ZXing.Mobile.MobileBarcodeScanner();
        //scanner.FlashButtonText = "Flash";
        //        scanner.TopText = "";
        //        scanner.BottomText = "";
        //        var result = await scanner.Scan();
        //Debug.WriteLine(scanner.Text);
    }
}

