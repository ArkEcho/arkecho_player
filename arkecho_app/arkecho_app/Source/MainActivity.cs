using Android.App;
using Android.Widget;
using Android.OS;

using System;
using System.Threading.Tasks;

using ZXing.Mobile;

namespace arkecho_app.source
{
    [Activity]
    public class MainActivity : BaseActivity
    {
        string qrCodeText_;
        MobileBarcodeScanner scanner_;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.MainActivity);

            setActionBarButtonBackHidden(true);

            // Connect Buttons
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Click += onPbConnectWithQrClicked;
            FindViewById<Button>(Resource.Id.pbConnectManually).Click += onPbConnectManuallyClicked;

            // Scanner initialisieren
            MobileBarcodeScanner.Initialize(Application);
            scanner_ = new MobileBarcodeScanner();
            qrCodeText_ = "";
        }

        private void setElementsEnabled(bool enabled)
        {
            FindViewById<Button>(Resource.Id.pbConnectWithQr).Enabled = enabled;
            FindViewById<Button>(Resource.Id.pbConnectManually).Enabled = enabled;
            FindViewById<TextView>(Resource.Id.teAddress).Enabled = enabled;
        }

        private void onPbConnectManuallyClicked(object sender, EventArgs e)
        {
            setElementsEnabled(false);
            string address = FindViewById<TextView>(Resource.Id.teAddress).Text;

            if (!ArkEchoWebSocket.checkIfURIAddressIsCorrect(address))
            {
                showMessageBoxEmptyWrongAddressField();
                setElementsEnabled(true);
                return;
            }

            connectAndOpenPlayer(address);
        }
        
        private async void onPbConnectWithQrClicked(object sender, System.EventArgs e)
        {
            setElementsEnabled(false);
            Task scan  = scanQrCode();
            await scan;

            if (!ArkEchoWebSocket.checkIfURIAddressIsCorrect(qrCodeText_))
            {
                showMessageBoxQrScanFailed();
                setElementsEnabled(true);
                return;
            }

            connectAndOpenPlayer(qrCodeText_);
        }

        private async void connectAndOpenPlayer(string address)
        {
            Task connect = ArkEchoWebSocket.connectWebSocket(address);
            await connect;

            setElementsEnabled(true);

            if (ArkEchoWebSocket.checkIfConnectionIsOpen()) StartActivity(typeof(PlayerActivity));
            else showMessageBoxNoConnection();
        }

        private void showMessageBoxQrScanFailed()
        {
            Toast mrToast = Toast.MakeText(this, Resource.String.ToastQrScanFailed, ToastLength.Short);
            mrToast.Show();
        }

        private void showMessageBoxEmptyWrongAddressField()
        {
            Toast mrToast = Toast.MakeText(this, Resource.String.ToastEmptyWrongAddress,ToastLength.Short);
            mrToast.Show();
        }

        private void showMessageBoxNoConnection()
        {
            Toast mrToast = Toast.MakeText(this, Resource.String.ToastNoConnection, ToastLength.Short);
            mrToast.Show();
        }

        private async Task scanQrCode()
        {
            try
            {
                scanner_.TopText = GetString(Resource.String.QrScannerTextTop);
                scanner_.BottomText = GetString(Resource.String.QrScannerTextBottom);

                var result = await scanner_.Scan();
                if (result != null) qrCodeText_ = result.Text;
                else qrCodeText_ = "";
            }
            catch (Exception ex)
            {
                ex.ToString();
            }
        }
    }
}

