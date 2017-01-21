using Android.App;
using Android.OS;

using System.Threading.Tasks;

namespace arkecho_app.source.activity
{
    [Activity(Theme = "@style/SplashScreen", MainLauncher = true, NoHistory = true)]
    public class SplashActivity : BaseActivity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            SetContentView(Resource.Layout.SplashActivity);
            // Create your application here
        }

        protected override async void OnResume()
        {
            base.OnResume();
            
            // Prepare WebSockets Connection
            Websockets.Droid.WebsocketConnection.Link();

            // Bisschen Verzögerung
            await Task.Delay(3000);

            StartActivity(typeof(MainActivity));
        }
    }
}