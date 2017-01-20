using System;

using Android.App;
using Android.OS;
using Android.Widget;

namespace arkecho_app.source
{
    [Activity(Label = "@string/ApplicationTitle", Icon = "@drawable/playerIcon")]
    public class BaseActivity : Activity
    {
        Button actionBarMenuButton;
        PopupMenu actionBarPopupMenu;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Action Bar
            ActionBar.SetDisplayShowCustomEnabled(true);
            ActionBar.SetCustomView(Resource.Layout.ActionBar);
            actionBarMenuButton = FindViewById<Button>(Resource.Id.pbActionBarMenu);
            actionBarMenuButton.Click += onPbActionBarMenuClicked;
            actionBarPopupMenu = new PopupMenu(this, actionBarMenuButton);
            actionBarPopupMenu.MenuInflater.Inflate(Resource.Menu.top_menu, actionBarPopupMenu.Menu);
            actionBarPopupMenu.MenuItemClick += onPopupMenuItemClicked;
            actionBarPopupMenu.DismissEvent += (s, arg) => { };
        }

        private void onPbActionBarMenuClicked(object sender, EventArgs e)
        {
            actionBarPopupMenu.Show();
        }

        private void onPopupMenuItemClicked(object sender, PopupMenu.MenuItemClickEventArgs e)
        {
            return;
        }
    }
}