#include "pch.h"

#include "Bot.h"

int main()
{
	std::string key;
	std::getline(std::ifstream("bot_apikey.txt"), key);
	if (key.empty()) {
		std::cout << "required file \"bot_apikey.txt\"\n";
		return -1;
	}

	Bot(key)
		.onMessage(
			[](Bot* bot, const Message& message)
			{
				auto text = message.text.value_or("without text");
				std::cout << "message: " << text << '\n';
				bot->sendMessage(message.chat.id, text, message.message_id);
			}
		)
		.run();
}
