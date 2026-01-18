#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

/*
 * Algorithm Challenge – Level 3 | #50 Delete Client by Account Number
 *
 * This program demonstrates how to safely delete a client record from a text file using a logical deletion strategy.
 *
 * Engineering Workflow:
 *   - Load all client records from file
 *   - Search for the target client by Account Number
 *   - Mark the client for deletion (logical delete)
 *   - Rewrite the file excluding the marked record
 *   - Reload updated data from file
 *
 * This approach ensures data integrity and avoids corrupting the file.
 */

// Client file name
const string ClientFileName1 = "Clients.txt";

// Structure that represents client data
struct ClientStructData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phon;
    double AccountBalance;
    bool MarkForDelete = false; // Logical deletion flag
};

// Splits a string using a delimiter
vector<string> SplitString(string S1, string Delim)
{
    vector<string> VecString;
    short Position = 0;
    string Token;

    while ((Position = S1.find(Delim)) != string::npos)
    {
        Token = S1.substr(0, Position);

        if (Token != "")
            VecString.push_back(Token);

        S1.erase(0, Position + Delim.length());
    }

    if (S1 != "")
        VecString.push_back(S1);

    return VecString;
}

// Converts a text line into a client structure
ClientStructData ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    ClientStructData Client;
    vector<string> ClientData = SplitString(Line, Seperator);

    Client.AccountNumber = ClientData[0];
    Client.PinCode = ClientData[1];
    Client.Name = ClientData[2];
    Client.Phon = ClientData[3];
    Client.AccountBalance = stod(ClientData[4]);

    return Client;
}

// Converts a client structure back into a text line
string ConvertRecordToLine(ClientStructData Client, string Seperator = "#//#")
{
    string ClientRecord = "";

    ClientRecord += Client.AccountNumber + Seperator;
    ClientRecord += Client.PinCode + Seperator;
    ClientRecord += Client.Name + Seperator;
    ClientRecord += Client.Phon + Seperator;
    ClientRecord += to_string(Client.AccountBalance);

    return ClientRecord;
}

// Loads all clients from the file
vector<ClientStructData> LoadClientsDataFromFile(string FileName)
{
    vector<ClientStructData> VecClients;
    fstream FileClients;

    FileClients.open(FileName, ios::in);

    if (FileClients.is_open())
    {
        string Line;
        ClientStructData Client;

        while (getline(FileClients, Line))
        {
            Client = ConvertLineToRecord(Line);
            VecClients.push_back(Client);
        }

        FileClients.close();
    }

    return VecClients;
}

// Prints a single client card
void PrintClientCard(ClientStructData Client)
{
    cout << "\nClient Information\n";
    cout << "-----------------------\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Full Name      : " << Client.Name << endl;
    cout << "Phone Number   : " << Client.Phon << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

// Searches for a client by account number
bool FindClientByAccountNumber(string AccountNumber,
                               vector<ClientStructData> vClients,
                               ClientStructData &Client)
{
    for (ClientStructData C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

// Marks a client for deletion
bool MarkClientForDeleteByAccountNumber(string AccountNumber,
                                        vector<ClientStructData> &vClients)
{
    for (ClientStructData &Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

// Rewrites the file excluding deleted clients
void SaveClientsDataToFile(string FileName,
                           vector<ClientStructData> vClients)
{
    fstream FileClients;
    FileClients.open(FileName, ios::out); // Overwrite mode

    if (FileClients.is_open())
    {
        for (ClientStructData Client : vClients)
        {
            if (!Client.MarkForDelete)
            {
                FileClients << ConvertRecordToLine(Client) << endl;
            }
        }
        FileClients.close();
    }
}

// Deletes a client by account number
bool DeleteClientByAccountNumber(string AccountNumber,
                                 vector<ClientStructData> &vClients)
{
    ClientStructData Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want to delete this client? (Y/N): ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientFileName1, vClients);

            // Reload updated data from file
            vClients = LoadClientsDataFromFile(ClientFileName1);

            cout << "\nClient deleted successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number ("
             << AccountNumber << ") not found.\n";
    }

    return false;
}

// Reads account number from user
string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "Enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

int main()
{
    vector<ClientStructData> vClients =
        LoadClientsDataFromFile(ClientFileName1);

    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

    return 0;
}
