using Android.App;
using Android.Widget;
using Android.OS;

using System;
using System.Collections.Generic;

using ZXing.Mobile;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace arkecho_app
{
    [Activity(Label = "@string/ApplicationTitle", MainLauncher = true, Icon = "@drawable/playerIcon")]
    public class MainActivityView : Activity
    {
        MainActivityModel model_;
        string qrCodeText_;

        List<string> items_;
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
            items_ = new List<string>();
            adapter = new ArrayAdapter<string>(this, Android.Resource.Layout.SimpleListItem1, items_);
            FindViewById<ListView>(Resource.Id.lvMessages).Adapter = adapter;

            // Create Model and Connect
            model_ = new MainActivityModel();
            model_.newMessageReceived += onNewMessageReceived;
        }

        private void onPbSendMessageClicked(object sender, EventArgs e)
        {
            string message = FindViewById<TextView>(Resource.Id.teMessage).Text;
            model_.sendMessage((int)MessageHandler.MESSAGETYPE.ECHO_TEST, message);
        }

        private void onNewMessageReceived(string message)
        {
            string msg = message;
            RunOnUiThread(() => adapter.Add(msg));
        }

        private async void onPbConnectClicked(object sender, System.EventArgs e)
        {
            Task scan  = scanQrCode();
            await scan;

            if (qrCodeText_ == "") return;
            JObject obj = JObject.Parse(qrCodeText_);
            int securityCode = obj["Security_Code"].ToObject<int>();
            string address = obj["Address"].ToObject<string>();

            //string address = FindViewById<TextView>(Resource.Id.teAddress).Text;
            Task connect = model_.connectWebSocket("ws://" + address);
            await connect;
            model_.sendMessage((int)MessageHandler.MESSAGETYPE.HANDSHAKE_SEC_CODE, securityCode.ToString());
        }
        private async Task scanQrCode()
        {
            try
            {
                MobileBarcodeScanner.Initialize(Application);
                MobileBarcodeScanner scanner = new MobileBarcodeScanner();
                scanner.FlashButtonText = "Blitz";
                scanner.TopText = "QR-Code Scanner";
                scanner.BottomText = "Halten sie den Roten Strich über den QR-Code";
                scanner.CancelButtonText = "Abbruch";
                var result = await scanner.Scan();

                qrCodeText_ = result.Text;
            }
            catch (Exception ex)
            {
            }
        }
    }
}

