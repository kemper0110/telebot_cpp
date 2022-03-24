#include "Bot.h"

Bot& Bot::onMessage(onMessageHandlerType&& func) {
	onMessageHandler = func;
	return *this;
}
Bot::Bot(const std::string& key) {
	const std::string api = "https://api.telegram.org/bot";
	api_key = api + key;
}

void Bot::checkUpdates() {
	static int offset = 0;

	const std::string update_command = "/getUpdates";

	curlpp::Easy req;
	req.setOpt(curlpp::options::Url(api_key + update_command));
	{
		curlpp::Forms fp;
		fp.push_back(new curlpp::FormParts::Content("offset", std::to_string(offset)));
		constexpr auto longpolling_timeout = 60;	// seconds
		fp.push_back(new curlpp::FormParts::Content("timeout", std::to_string(longpolling_timeout)));
		req.setOpt(curlpp::options::HttpPost(fp));
	}

	std::stringstream res;
	res << req;

	json data = json::parse(res);

	if (not data.contains("result")) return;

	json result_array = data["result"];
	if (!result_array.empty()) {
		offset = result_array.back()["update_id"].get<int>() + 1;
		std::ofstream("log.txt", std::ios::app) << std::setw(4) << result_array << std::endl;
		for (auto v : result_array) {
			// only to handle messages
			if(auto i = v.find("message"); i != v.end())
				onMessageHandler(this, i->get<Message>());
		}
	}
	//std::cout << "long_polling timeout\n";
}
Bot& Bot::sendMessage(int chat_id, std::string text, int reply_to_message_id = 0) {
	const std::string command = "/sendMessage";
	curlpp::Easy request;
	request.setOpt(new curlpp::options::Url(api_key + command));
	{
		curlpp::Forms fp;
		if (reply_to_message_id)
			fp.push_back(new curlpp::FormParts::Content("reply_to_message_id", std::to_string(reply_to_message_id)));
		fp.push_back(new curlpp::FormParts::Content("chat_id", std::to_string(chat_id)));
		fp.push_back(new curlpp::FormParts::Content("text", text));
		request.setOpt(curlpp::options::HttpPost(fp));
	}
	std::stringstream ss;
	ss << request;
	json res = json::parse(ss);
	
	if (not res["ok"].get<bool>())
		std::cout << std::setw(4) << res << std::endl;
	return *this;
}

void Bot::run() {
	while (1) 
		checkUpdates();
}
