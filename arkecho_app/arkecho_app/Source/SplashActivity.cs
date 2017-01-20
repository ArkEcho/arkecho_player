using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System.Threading.Tasks;

namespace arkecho_app.source.Source
{
    [Activity(Theme = "@style/SplashScreen", MainLauncher = true, NoHistory = true)]
    public class SplashActivity : Activity
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

            // Bisschen Verz�gerung
            await Task.Delay(3000);

            StartActivity(typeof(MainActivity));
        }
    }
}