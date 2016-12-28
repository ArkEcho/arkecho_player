using Android.App;
using Android.Widget;
using Android.OS;

using System;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

using ZXing.Mobile;

namespace arkecho_app
{
    [Activity(Label = "@string/ApplicationTitle", MainLauncher = true, Icon = "@drawable/playerIcon")]
    public class MainActivity : Activity
    {
        string qrCodeText_;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView (Resource.Layout.MainActivity);

            // Connect Buttons
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Click += onPbConnectWithQrClicked;
            FindViewById<Button>(Resource.Id.pbConnectManually).Click += onPbConnectManuallyClicked;

            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();
        }
        
        private void setElementsEnabled(bool enabled)
        {
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Enabled = enabled;
            FindViewById<Button>(Resource.Id.pbConnectManually).Enabled = enabled;
            FindViewById<TextView>(Resource.Id.teAddress).Enabled = enabled;
        }

        private async void onPbConnectManuallyClicked(object sender, EventArgs e)
        {
            setElementsEnabled(false);
            string address = FindViewById<TextView>(Resource.Id.teAddress).Text;

            if (!checkURIAddress(address))
            {
                showMessageBoxEmptyWrongAddressField();
                setElementsEnabled(true);
                return;
            }

            Task connect = ArkEchoWebSocket.connectWebSocket(address);
            await connect;

            setElementsEnabled(true);
            checkConnectionAndOpenPlayer();
        }
        
        private async void onPbConnectWithQrClicked(object sender, System.EventArgs e)
        {
            setElementsEnabled(false);
            Task scan  = scanQrCode();
            await scan;

            if (qrCodeText_ == "") return;

            string address = qrCodeText_;

            if (!checkURIAddress(address))
            {
                showMessageBoxEmptyWrongAddressField();
                setElementsEnabled(true);
                return;
            }

            Task connect = ArkEchoWebSocket.connectWebSocket(address);
            await connect;

            setElementsEnabled(true);
            checkConnectionAndOpenPlayer();
        }

        private void checkConnectionAndOpenPlayer()
        {
            if (ArkEchoWebSocket.connectionIsOpen())
            {
                StartActivity(typeof(PlayerActivity));
            }
            else
            {
                showMessageBoxNoConnection();
            }
        }

        private bool checkURIAddress(string address)
        {
            if (address == "") return false;
            var regex = @"[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,2}\:[0-9]{4}";
            return Regex.Match(address, regex).Success;
        }

        private void showMessageBoxEmptyWrongAddressField()
        {
            AlertDialog.Builder alert = new AlertDialog.Builder(this);
            alert.SetTitle("Achtung:");
            alert.SetMessage("Bitte geben sie die Adresse korrekt und vollständig ein!");
            alert.SetPositiveButton("Ok", (senderAlert, args) => { });
            alert.Show();
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

