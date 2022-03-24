#ifndef BOT_H
#define BOT_h


#include "pch.h"

#include "Message.h"


class Bot
{
private:
	void checkUpdates();
public:
	using onMessageHandlerType = std::function<void(Bot*, const Message&)>;

	Bot(const std::string& key);

	Bot& onMessage(onMessageHandlerType&& func);

	Bot& sendMessage(int chat_id, std::string text, int reply_to_message_id);

	void run();
private:
	onMessageHandlerType onMessageHandler;
	curlpp::Cleanup cleanup;
	std::string api_key;
};

#endif