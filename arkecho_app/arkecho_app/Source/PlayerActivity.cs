using Android.App;
using Android.OS;
using System.Threading.Tasks;

namespace arkecho_app.source
{
    [Activity(Label = "@string/ApplicationTitle")]
    public class PlayerActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.PlayerActivity);

            ArkEchoWebSocket.webSocketConnectionClosed += OnBackPressed;
        }

        public override async void OnBackPressed()
        {
            ArkEchoWebSocket.disconnectWebSocket();
            await Task.Delay(10);
            Finish();
            //RunOnUiThread(() => base.OnBackPressed());
        }
    }
}