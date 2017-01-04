using Android.Graphics.Drawables;
using System;
using System.IO;

using Newtonsoft.Json.Linq;

namespace arkecho_app.source
{
    class MessageHandler
    {
        private const string JSON_TYPE = "Type";
        private const string JSON_MESSAGE = "Message";

        public enum MESSAGETYPE
        {
            MT_NOTDEFINED = 0,
            MT_ECHO_TEST,
            MT_PLAY_PAUSE,
            MT_BACKWARD,
            MT_FORWARD,
            MT_SONG_ACTUAL
        }

        public static string createMessage(int messageType, string message)
        {
            JObject obj = new JObject();
            obj[JSON_TYPE] = messageType;
            obj[JSON_MESSAGE] = message;
            return obj.ToString();
        }

        public static int handleReceivedMessage(ref string message)
        {
            JObject obj = JObject.Parse(message);
            int messageType = obj[JSON_TYPE].ToObject<int>();
            message = obj[JSON_MESSAGE].ToObject<string>();
            return messageType;
        }

        public static Drawable base64ToImage(string base64String)
        {
            MemoryStream stream = new MemoryStream();
            byte[] barray = Convert.FromBase64String(base64String);
            stream.Write(barray, 0, barray.Length);
            Drawable drawableImage = Drawable.CreateFromStream(stream, null);
            return drawableImage;
        }
    }
}