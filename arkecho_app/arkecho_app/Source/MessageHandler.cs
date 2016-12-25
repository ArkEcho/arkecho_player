using Newtonsoft.Json.Linq;

namespace arkecho_app
{
    class MessageHandler
    {
        private const string JSON_TYP = "Typ";
        private const string JSON_MESSAGE = "Message";

        public static string createMessage(int messageType, string message)
        {
            JObject obj = new JObject();
            obj[JSON_TYP] = messageType;
            obj[JSON_MESSAGE] = message;
            return obj.ToString();
        }

        public static int handleReceivedMessage(ref string message)
        {
            JObject obj = JObject.Parse(message);
            int messageType = obj[JSON_TYP].ToObject<int>();
            message = obj[JSON_MESSAGE].ToObject<string>();
            return messageType;
        }
    }
}