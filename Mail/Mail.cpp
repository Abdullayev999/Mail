#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <exception>
#pragma warning (disable: 4996)


struct User_data
{
	char m_name[30]{};
	char  m_login[30]{};
	char  m_password[30]{};
	char  m_repeat_password[30]{};
	char m_path[100]{};

	void addPath(char* path)
	{
		strcpy(path, "text");
		int userPathSize = strlen(path);
        const int idSize = 10;
		for (size_t i = userPathSize; i < userPathSize + idSize; i++)
		{
			path[i] = rand() % ('z' - 'a' + 1) + 'a';
		}
		path[userPathSize + idSize] = '\0';
		strcat(path, ".txt");
	}

	friend  std::istream& operator>>(std::istream& is, User_data& n)
	{
		std::cout << "\n\nEnter name     : ";
		std::cin >> n.m_name;

		std::cout << "Enter login    : ";
		std::cin >> n.m_login;

		std::cout << "Enter password : ";
		std::cin >> n.m_password;

		std::cout << "Repeat password : ";
		std::cin >> n.m_repeat_password;

		n.addPath(n.m_path);
		return is;
	}
};

class Users
{
public:
	Users() = default;

	void print_friend()
	{
		std::cout << "Friend list\n# \tName\t\t\tLogin\n\n";

		for (int i = 0; i < m_index; ++i) {
			std::cout << i << "\t" << m_users[i].m_name << "\t\t\t" << m_users[i].m_login << '\n';
		}
	}

	bool add(const User_data& user)
	{
		if (m_size == m_index)capacity_up();

		if (authentication(user.m_login)) {
			return false;
		}

		m_users[m_index++] = user;

		return true;
	}

	void deleted(const int index)
	{
		if (m_index <= index)
		{
			User_data* tmp = new User_data[m_index - 1];
			for (int i = 0, b = 0; i < m_index; ++i)
			{
				if (i != index) {
					tmp[b] = m_users[i];
					b++;
				}
			}
			delete[] m_users;
			m_index--;
			m_users = tmp;
		}
		else {
			throw std::range_error("Out of range!");
		}
	}

	bool authentication(const char* login)
	{
		for (int i = 0; i < m_index; ++i) {
			if (!strcmp(m_users[i].m_login , login))
				return true;
		}
		return false;
	}

	bool sign_in(const char* login, const char* password)
	{
		for (int i = 0; i < m_index; ++i) {
			if (!strcmp(m_users[i].m_login, login) && !strcmp(m_users[i].m_password, password))
				return true;
		}
		return false;
	}

	const User_data& get_user(const char* login, const char* password)
	{
		for (int i = 0; i < m_index; ++i)
		{
			if (!strcmp(m_users[i].m_login , login) && !strcmp(m_users[i].m_password , password))
			{
				return m_users[i];
			}
		}
		throw std::invalid_argument("Not user!");
	}

	~Users()
	{
		delete[] m_users;
	}

	Users(const Users& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = new User_data[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_users[i] = other.m_users[i];
	}

	Users(Users&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = other.m_users;
		other.m_users = nullptr;
	}

	Users& operator=(const Users& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = new User_data[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_users[i] = other.m_users[i];

		return *this;
	}

	Users& operator=(Users&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = other.m_users;
		other.m_users = nullptr;

		return *this;
	}

	char* get_path(const char* login)
	{
		for (int i = 0; i < m_index; ++i)
		{
			if (!strcmp(m_users[i].m_login , login))
			{
				return m_users[i].m_path;
			}
		}
		throw std::invalid_argument("Not user!");
	}

	int get_size()
	{
		return m_index;
	}
private:

	friend std::ostream& operator<<(std::ostream& os, const Users& users)
	{
		os << "#\t\tName\t\t\tLogin\t\t\tPassword\t\tPath\n";
		for (int i = 0; i < users.m_index; ++i)
		{
			os << i << "\t\t" << users.m_users[i].m_name << "\t\t\t";
			os << users.m_users[i].m_login << "\t\t\t" << users.m_users[i].m_password << "\t\t\t" << users.m_users[i].m_path << '\n';
		}
		return os;
	}

	void capacity_up()
	{
		m_size += m_capacity;

		User_data* tmp = new User_data[m_size];
		for (int i = 0; i < m_index; ++i)
		{
			tmp[i] = m_users[i];
		}
		delete[] m_users;
		m_users = tmp;
	}

	int m_size = 5;
	int m_index = 0;
	int m_capacity = 5;
	User_data* m_users = new User_data[m_size];
};

enum
{
	COME_BACK = -1,
	EXIT,
	SIGN_UP = 1,
	SIGN_IN,
	SEND_MESSAGE = 1,
	INBOX,
	PRINT_ALL = 3
};

int main()
{
	srand(time(nullptr));
	
	std::string m_path = "mail.txt";

    Users users;
	User_data sign_up, user;
	int action;
	char login[50];
	char password[50];
	char path[100];

	std::ios::openmode mode = std::ios::binary | std::ios::app | std::ios::in | std::ios::out;
	std::fstream inputBin(m_path, mode);
	while (inputBin.read(reinterpret_cast<char*>(&user), sizeof(User_data)))
	{
		users.add(user);
	}
	inputBin.close();


	do
	{
		std::cout << R"(
0 - Exit
1 - Sign up 
2 - Sign in
3 - Print all user
)";
		std::cin >> action;
		system("cls");
		
		if (action == PRINT_ALL)
		{
			if (users.get_size())
			{
				std::cout << users;
			}
			else
			{
				std::cout << "\nNot users!\n";
			}
		}
		else if (action == SIGN_UP)
		{
			std::cout << "\n\n<< Fill in the registration User >> \n\n";
			std::cin.ignore();
			std::cin >> sign_up;

			if (!strcmp(sign_up.m_password, sign_up.m_repeat_password))
			{
				if (users.add(sign_up))
				{
					std::fstream outputBin(m_path, mode);
					outputBin.write(reinterpret_cast<char*>(&sign_up), sizeof(User_data));
					outputBin.close();
					std::cout << "\nRegistration was successful\n";
				}
				else
				{
					std::cout << "A user with such data exists\n";
				}
			}
			else
			{
				std::cout << "Password mismatch\n";
			}
		}
		else if (action == SIGN_IN)
		{
			std::cout << "\nEnter data for Sign in\n";
			std::cout << "Login     :  ";
			std::cin.ignore();
			std::cin >> login;

			std::cout << "Password  :  ";
			std::cin>>password;
			system("cls");

			if (users.sign_in(login, password))
			{
				try
				{
					sign_up = users.get_user(login, password);
				}
				catch (std::invalid_argument& ex)
				{
					std::cout << ex.what();
				}
				std::cout << "Hello ";

				do
				{
					std::cout <<"  <<  "<< sign_up.m_name <<"  >>  "<< std::endl;
					std::cout << R"(
-1 - Come back
 0 - Exit
 1 - Send message
 2 - Inbox
 3 - Print friend						
						
)";
					std::cin >> action;
					system("cls");

					if (action == PRINT_ALL)
					{
						users.print_friend();
					}
					else if (action == COME_BACK || action == EXIT)
					{
						break;
					}
					else if (action == SEND_MESSAGE)
					{
						std::cout << "Who do you want to send the message to\nEnter login\n";
						std::cin.ignore();
						std::cin >> login;
						try
						{
							strcpy(path, users.get_path(login));
							
								std::fstream output(path, mode);
								if (output) {
									std::cout << "Chat is opened!\nEnter message\n\nSend message - 0\n\n";
									std::string text;
									output << sign_up.m_name << " : ";
									do
									{
										std::getline(std::cin, text);

										if (text == "0")
										{
											std::cout << "Message send\n";
											break;
										}
										output << text << "\n";
									} while (true);
									output << "\n\n";
									output.close();
								}
							
						}
						catch (std::exception& ex)
						{
							std::cout << ex.what() << '\n';
						}
					}
					else if (action == INBOX)
					{
						std::fstream input(sign_up.m_path, mode);
						if (input) {
							input.seekg(0);
							std::string line;
							while (std::getline(input, line))
							{
								std::cout << line << '\n';
							}
							input.close();
						}
					}
					std::cout << "\n\n";
				} while (true);
			}
			else
			{
				std::cout << "Data entered incorrectly\n";
			}
		}
		else if (action == EXIT)
		{
			break;
		}
		else
		{
			std::cout << "\nIncorrect choice\n";
		}
	} while (action != EXIT);


}

