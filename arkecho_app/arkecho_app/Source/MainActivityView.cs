using Android.App;
using Android.Widget;
using Android.OS;

using System;

using ZXing.Mobile;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace arkecho_app
{
    [Activity(Label = "@string/ApplicationTitle", MainLauncher = true, Icon = "@drawable/playerIcon")]
    public class MainActivityView : Activity
    {
        private ArkEchoWebSocket webSocket_;
        string qrCodeText_;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            // Set our view from the "main" layout resource
            SetContentView (Resource.Layout.Main);

            // Connect Buttons
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Click += onPbConnectWithQrClicked;
            FindViewById<Button>(Resource.Id.pbConnectManually).Click += onPbConnectManuallyClicked;

            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();

            // Create Model and Connect
            webSocket_ = new ArkEchoWebSocket();
        }

        private async void onPbConnectManuallyClicked(object sender, EventArgs e)
        {
            string address = FindViewById<TextView>(Resource.Id.teAddress).Text;

            if (address == "") return;
            Task connect = webSocket_.connectWebSocket("ws://" + address);
            await connect;

            checkConnectionAndOpenPlayer();
        }
        
        private async void onPbConnectWithQrClicked(object sender, System.EventArgs e)
        {
            Task scan  = scanQrCode();
            await scan;

            if (qrCodeText_ == "") return;

            string address = qrCodeText_;
            
            Task connect = webSocket_.connectWebSocket("ws://" + address);
            await connect;

            checkConnectionAndOpenPlayer();
        }

        private void checkConnectionAndOpenPlayer()
        {
            if (webSocket_.connectionIsOpen())
            {

            }
            else
            {
                showMessageBoxNoConnection();
            }
        }

        private void showMessageBoxNoConnection()
        {
            AlertDialog.Builder alert = new AlertDialog.Builder(this);
            alert.SetTitle("Fehler:");
            alert.SetMessage("Keine Verbindung zum ArkEcho Player möglich!");
            alert.SetPositiveButton("Ok", (senderAlert, args) => { });
            alert.Show();
        }

        private async Task scanQrCode()
        {
            try
            {
                MobileBarcodeScanner.Initialize(Application);
                MobileBarcodeScanner scanner = new MobileBarcodeScanner();
                scanner.UseCustomOverlay = true;
                scanner.TopText = "QR-Code Scanner";
                scanner.BottomText = "Halten sie den Roten Strich über den QR-Code";

                var result = await scanner.Scan();
                qrCodeText_ = result.Text;
            }
            catch (Exception ex)
            {
            }
        }
    }
}

