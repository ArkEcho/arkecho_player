using Android.App;
using Android.Widget;
using Android.OS;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;

namespace arkecho_app
{
    [Activity(Label = "ArkEcho", MainLauncher = true, Icon = "@drawable/playerIcon")]
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

            FindViewById<Button>(Resource.Id.pbConnect).Click += onPbConnectClicked;
            FindViewById<Button>(Resource.Id.pbSendMessage).Click += onPbSendMessageClicked;

            items = new List<string>();
            adapter = new ArrayAdapter<string>(this, Android.Resource.Layout.SimpleListItem1, items);
            FindViewById<ListView>(Resource.Id.lvMessages).Adapter = adapter;

            Websockets.Droid.WebsocketConnection.Link();

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
        }

        private void onPbConnectClicked(object sender, System.EventArgs e)
        {
            String address = FindViewById<TextView>(Resource.Id.teAddress).Text;
            model_.connectWebSocket("ws://" + address);
        }
    }
}

