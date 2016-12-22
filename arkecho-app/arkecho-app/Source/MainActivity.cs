using Android.App;
using Android.Widget;
using Android.OS;

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

            FindViewById<Button>(Resource.Id.pbClickMe).Click += PbClickMe_Click;
        }

        private void PbClickMe_Click(object sender, System.EventArgs e)
        {
            TextView lblHelloWorld = FindViewById<TextView>(Resource.Id.lblHelloWorld);
            lblHelloWorld.Text = "Test";
        }
    }
}

