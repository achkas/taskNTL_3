#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <windows.h>



enum class MessageType
{
	Warning,
	Error,
	FatalError,
	Unknown
};

class LogMessage
{
private:
	MessageType m_type;
	std::string m_message;

public:
	LogMessage(MessageType type, const std::string& message) : m_type(type), m_message(message) {}	

	MessageType type() const
	{
		return m_type;
	}

	const std::string& message() const
	{
		return m_message;
	}

	const bool operator == (const LogMessage& other)
	{
		return this->m_type == other.m_type;
	}

};



class HandlerLogMessag 
{
public:
	virtual void setHandler(HandlerLogMessag* handler) = 0;

	virtual void handleMessage(const LogMessage& message) = 0;
};

class WarningHandler : public HandlerLogMessag//1 должен напечатать сообщение в консоль
{
private:
	HandlerLogMessag* nextHandler{};

public:
	void
		setHandler(HandlerLogMessag* handler) override
	{
		nextHandler = handler;
	}

	void handleMessage(const LogMessage& message) override
	{
		LogMessage* ref_Warning = new LogMessage(MessageType::Warning, " ");		

		if (message == *ref_Warning)
		{
			std::cout << "Warning: " + message.message() << std::endl;
		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleMessage(message);
		}

		delete ref_Warning;
	}
};

class ErrorHandler : public HandlerLogMessag//2 должен записать её в файл по указанному пути.
{
private:
	HandlerLogMessag* nextHandler{};

public:
	void
		setHandler(HandlerLogMessag* handler) override
	{
		nextHandler = handler;
	}

	void handleMessage(const LogMessage& message) override
	{
		LogMessage* ref_Error = new LogMessage(MessageType::Error, " ");		

		if (message == *ref_Error)
		{
			//std::cout << "Ошибка " << std::endl;
			std::ofstream rf;
			rf.open("Текст.txt");
			if (!rf.is_open()) { std::cout << "not open!"; }
			rf << "Error: " + message.message() << std::endl;
			rf.close();
		}
		else if (nextHandler != nullptr)
		{
			nextHandler->handleMessage(message);
		}

		delete ref_Error;
	}
};

class FatalerrorHandler : public HandlerLogMessag
	//3 должен выбросить исключение с текстом сообщения,
	//  потому что предполагается, что программа должна прекратить выполнение после возникновения фатальной ошибки.
{
private:
	HandlerLogMessag* nextHandler{};

public:
	void
		setHandler(HandlerLogMessag* handler) override
	{
		nextHandler = handler;
	}

	void handleMessage(const LogMessage& message) override
	{
		LogMessage* ref_Fatalerror = new LogMessage(MessageType::FatalError, " ");
		
		try
		{
			if (message == *ref_Fatalerror)
			{				
				throw std::logic_error("FatalError: " + message.message() + "\n");
			}
			else if (nextHandler != nullptr)
			{
				nextHandler->handleMessage(message);
			}
		}

		catch (std::exception& e)
		{
			std::cout << e.what();
			exit(1);
		}

		delete ref_Fatalerror;

	}
};

class UnknownmessageHandler : public HandlerLogMessag//4 выбросить исключение с текстом о необработанном сообщении
{
private:
	HandlerLogMessag* nextHandler{};

public:
	void
		setHandler(HandlerLogMessag* handler) override
	{
		nextHandler = handler;
	}

	void handleMessage(const LogMessage& message) override
	{
		LogMessage* ref_Unknownmessage = new LogMessage(MessageType::Unknown, " ");		

		try
		{
			if (message == *ref_Unknownmessage)
			{				
				throw std::logic_error("Unknown_message: " + message.message() + "\n");
			}
			else if (nextHandler != nullptr)
			{
				nextHandler->handleMessage(message);
			}
		}

		catch (std::exception& e)
		{
			std::cout << e.what();
		}

		delete ref_Unknownmessage;
	}
};


int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	LogMessage war(MessageType::Warning, "Это предупреждение");
	LogMessage err(MessageType::Error, "Это ошибка");
	LogMessage fat_err(MessageType::FatalError, "Это критическая ошибка");
	LogMessage unk(MessageType::Unknown, "Это неизвестный тип");


	HandlerLogMessag* warning = new WarningHandler;
	HandlerLogMessag* error = new ErrorHandler;
	HandlerLogMessag* fatal_error = new FatalerrorHandler;
	HandlerLogMessag* unknown_message = new UnknownmessageHandler;

	warning->setHandler(error);
	error->setHandler(fatal_error);
	fatal_error->setHandler(unknown_message);

	warning->handleMessage(war);
	warning->handleMessage(err);	 
	warning->handleMessage(unk);
    warning->handleMessage(fat_err);

	delete fatal_error;
	delete error;
	delete warning;
	delete unknown_message;

		return 0;
}