#include "iostream"
#include "string"
#include "vector"
#include "algorithm"
#include "cstring"
  
class Contact;

bool compareContactsByName(Contact *contact1, Contact* contact2);

bool emailValidator(const std::string& email)
{
    if( email.find('@') == std::string::npos )
        return false;

    if( email.find(".com") != email.length() - 4 )
        return false;

    if( email.find('@') == 0 )
        return false;

    if(email.find('.') == email.find('@') + 1)
        return false;

    if( std::count(email.begin(), email.end(), '@') > 1)
        return false;


    return true;
}

void dispMenu(const Contact* cnt);


class Contact
{
protected: // not private just for the sake of access allow, a change.

    std::string _name;
    std::string _pNo;
    std::string _email;

public:
    Contact(std::string& pNo, std::string& name, std::string& email)
    {
        _name = name;
        _pNo = pNo;
        _email = email;
    }

    virtual void getInfo()
    {
        std::cout << "\nName: " + _name + "\nPhoneNumber: " + _pNo + "\nEmail: " + _email << "\n";

        // to just add a newline, instead of printing the other next details of other classes
        if(dynamic_cast<Contact*> (this))
            std::cout << "\n";
    }

    std::string getName()
    {
        return _name;
    }

    void call()
    {
        std::cout << "Calling " + _name + " at " << _pNo << "...\n\n";
    }

    void message()
    {
        std::cout << "Messaging " + _name + " at " << _pNo << "...\n\n";
    }


public:
    void changeNumber(std::string& pNo)
    {
        _pNo = pNo;
    }

    void changeName(std::string& name)
    {
        _name = name;
    }

    void changeEmail(std::string& email)
    {
        _email = email;
    }
};


class BusinessContact : public Contact
{
private:
    std::string _businessName;
    std::string _officeAddress;
    std::string _busEmail;

public:
    BusinessContact(std::string& pNo, std::string& name, std::string& email, std::string& businessName,
                                      std::string& officeAddress, const std::string& busEmail = " ") : Contact(pNo, name, email)
    {
        _businessName = businessName;
        _officeAddress = officeAddress;
        _busEmail = busEmail;
    }

    void getInfo() override
    {
        this->Contact::getInfo();

        std::cout <<  "Business Name: " + _businessName + "\nOffice Address: " + _officeAddress +
                                                                             "\nBusiness Email: " + _busEmail << "\n\n";
    }

    void changeBusinessName(std::string& businessName)
    {
        _businessName = businessName;
    }

    void changeOfficeAddress(std::string& officeAddress)
    {
        _officeAddress = officeAddress;
    }

    void changeBusEmail(std::string& busEmail)
    {
        _busEmail = busEmail;
    }
};


class PersonalContact : public Contact
{
private:
    std::string _relationship;

public:
    PersonalContact(std::string& pNo, std::string& name, std::string& email, std::string& relationship) : Contact(pNo, name, email)
    {
        _relationship = relationship;
    }

    void getInfo() override
    {
        this->Contact::getInfo();

        std::cout << "Relationship: " + _relationship << "\n\n";
    }

    void changeRelationship(std::string& relationship)
    {
        _relationship = relationship;
    }
};


class PhoneBook
{
    std::vector<Contact*> myContacts;

public:
    void addContact()
    {
        int choiceBusiness, choicePersonal;

        std::string name, pNo, email, businessName, officeAddress, busEmail, relationship;

        std::cout << "Enter Name: ";
        std::cin.ignore();

        std::getline(std::cin, name, '\n');

        bool validContact = false;
        while(!validContact)
        {
            try
            {
                std::cout << "Enter Phone Number: ";
                std::cin >> pNo;

                if(pNo.size() != 10)
                    throw std::runtime_error("Error: Length of Contact should be 10");
                else
                {
                    validContact = true;
                    break;
                }
            }
            catch(const std::runtime_error& er)
            {
                std::cout << er.what() << "\n\n";
                std::cin.ignore();
            }
        }
        bool validEmail = false;

        while(!validEmail)
        {
            try
            {
                std::cout << "Enter Email: ";
                std::cin >> email;

                validEmail = emailValidator(email);

                if(!validEmail)
                    throw std::runtime_error("Invalid Email Format.\nEmail should be of the form \"name@domain.com\"");
                else
                    break;
            }
            catch(std::runtime_error& e)
            {
                std::cout << e.what() << "\n\n";
                std::cin.ignore();
            }
        }

        std::cout << "\nIs this a Business Contact? (1/0): ";
        std::cin >> choiceBusiness;

        if(choiceBusiness == 1)
        {
            std::cout << "Enter Business Name: ";
            std::cin.ignore();
            std::getline(std::cin, businessName, '\n');

            std::cout << "Enter Office Address: ";
            std::getline(std::cin, officeAddress, '\n');


            validEmail = false;
            while(!validEmail)
            {
                try
                {
                    std::cout << "Enter Business Email: ";
                    std::cin >> busEmail;

                    validEmail = emailValidator(busEmail);

                    if(!validEmail)
                        throw std::runtime_error("Invalid Email Format.\nEmail should be of the form \"name@domain.com\"");
                    else
                        break;
                }
                catch(std::runtime_error& e)
                {
                    std::cout << e.what() << "\n\n";
                    std::cin.ignore();
                }
            }
        }

        if(choiceBusiness != 1)
        {
            std::cout << "\nAny Personal Relationship? (1/0): ";
            std::cin >> choicePersonal;

            if(choicePersonal == 1)
            {
                std::cout << "Enter Relationship: ";
                std::cin >> relationship;
            }
        }

        if(choiceBusiness == 1)
            myContacts.push_back(new BusinessContact(pNo, name, email, businessName, officeAddress, busEmail));
        else if(choicePersonal == 1)
            myContacts.push_back(new PersonalContact(pNo, name, email, relationship));
        else
            myContacts.push_back(new Contact(pNo, name, email));
    }

    void displayContacts()
    {
        std::sort(myContacts.begin(), myContacts.end(), compareContactsByName);

        if(myContacts.empty())
            std::cout << "No Contacts to Display\n\n";
        else
            std::cout << "Displaying...\n\n";
        for(auto it : myContacts)
            it->getInfo();
    }

    void searchContact(const std::string& name)
    {
        bool flag = false;
        Contact* i = nullptr;

        for(auto it : myContacts)
        {
            auto p = it->getName().data();
            auto q = name.data();

            if(strcasecmp(p, q) == 0)
            {
                std::cout << "Contact found!\n";
                flag = true;
                i = it;
                break;
            }
        }

        if(flag)
        {
            int choice = 0;
            std::cout << "What do you want to do with " << i->getName() << "?\n\n";

            std::cout << "1. Call\n"
                         "2. Message\n\n";
            std::cin >> choice;

            if(choice == 1)
                i->call();
            else if(choice == 2)
                i->message();
            else
                std::cout << "Invalid Choice!\n\n";
        }
        else if(!flag)
            std::cout << "Contact not found!\n";
    }

    void deleteContact(const std::string& name)
    {
        for(auto it = myContacts.begin() ; it != myContacts.end() ; it++)
        {
            if((*it)->getName() == name)
            {
                int choice;
                std::cout << "Are you sure you want to delete? (0/1) \n\n";
                std::cin >> choice;

                if(choice)
                {
                    myContacts.erase(it);
                    break;
                }
                else
                    std::cout << "Contact not deleted because you mis-clicked!!\n\n";
            }
        }
    }

    void editContact(std::string& name)
    {
        bool found = false;
        std::vector<Contact*>::iterator it = myContacts.begin();

        for( ; it != myContacts.end() ; it++)
        {
            auto p = (*it)->getName().data();
            auto q = name.data();

            if( strcasecmp(p, q) == 0 )
            {
                found = true;
                break;
            }
        }

        if(found)
        {
            std::cout << "What do you want to edit?\n";

            dispMenu(*it);

            int choice;
            std::cin >> choice;
            std::cout << '\n';

            std::cin.ignore();

            if(choice == 1)
            {
                bool validContact = false;
                std::string pNo;

                while(!validContact)
                {
                    try
                    {
                        std::cout << "Enter new Phone Number: ";
                        std::cin >> pNo;

                        if(pNo.size() != 10)
                            throw std::runtime_error("Error: Length of Contact should be 10");
                        else
                        {
                            validContact = true;
                            break;
                        }
                    }
                    catch(const std::runtime_error& er)
                    {
                        std::cout << er.what() << "\n\n";
                        std::cin.ignore();
                    }
                }

                (*it)->changeNumber(pNo);
            }
            else if(choice == 2)
            {
                std::string newName;

                std::cout << "Enter new Name: ";
                std::getline(std::cin, newName, '\n');

                (*it)->changeName(newName);
            }
            else if(choice == 3)
            {
                bool validEmail = false;
                std::string email;

                while(!validEmail)
                {
                    try
                    {
                        std::cout << "Enter new Email: ";
                        std::cin >> email;

                        validEmail = emailValidator(email);

                        if(!validEmail)
                            throw std::runtime_error("Invalid Email Format.\nEmail should be of the form \"name@domain.com\"");
                        else
                            break;
                    }
                    catch(const std::runtime_error& er)
                    {
                        std::cout << er.what() << "\n\n";
                        std::cin.ignore();
                    }
                }

                (*it)->changeEmail(email);
            }
            else if(choice == 4)
            {
                if( auto* bContact = dynamic_cast<BusinessContact*> (*it) )
                {
                    std::string businessName;

                    std::cout << "Enter new Business Name: ";
                    std::getline(std::cin, businessName);

                    bContact->changeBusinessName(businessName);
                }
                else if( auto* pContact = dynamic_cast<PersonalContact*> (*it) )
                {
                    std::string relationship;

                    std::cout << "Enter new Relationship: ";
                    std::getline(std::cin, relationship);

                    pContact->changeRelationship(relationship);
                }
            }
            else if(choice == 5)
            {
                if( auto* bContact = dynamic_cast<BusinessContact*> (*it) )
                {
                    std::string officeAddress;

                    std::cout << "Enter new Office Address: ";
                    std::getline(std::cin, officeAddress);

                    bContact->changeOfficeAddress(officeAddress);
                }
            }
            else if(choice == 6)
            {
                if( auto* bContact = dynamic_cast<BusinessContact*> (*it) )
                {
                    std::string busEmail;

                    try
                    {
                        std::cout << "Enter new Business Email: ";
                        std::cin >> busEmail;

                        bool valid = emailValidator(busEmail);

                        if(!valid)
                            throw std::runtime_error("Invalid Email");
                    }
                    catch(std::runtime_error& e)
                    {
                        std::cout << e.what() << "\n\n";
                        std::cout << "Enter Email: ";
                        std::cin >> busEmail;
                    }
                    bContact->changeBusEmail(busEmail);
                }
            }
            else
                std::cout << "Invalid Choice!\n\n";

        }
        else
        {
            std::cout << "Contact Not Found\n\n";
        }
    }

    ~PhoneBook()
    {
        for (auto it : myContacts)
        {
            delete it;
        }

        myContacts.clear();
    }

};

void dispMenu(const Contact* cnt)
{
    if( auto* bContact = dynamic_cast<const BusinessContact*> (cnt) )
        std::cout << "1. Phone Number\n2. Name\n3. Email\n4. Business Name\n5. Office Address\n6. Business Email\n\n";

    else if( auto* pContact = dynamic_cast<const PersonalContact*> (cnt) )
        std::cout << "1. Phone Number\n2. Name\n3. Email\n4. Relationship\n\n";

    else if( auto* contact = dynamic_cast<const Contact*> (cnt) )
        std::cout << "1. Phone Number\n2. Name\n3. Email\n\n";

}

bool compareContactsByName(Contact *contact1, Contact* contact2)
{
    return contact1->getName() < contact2->getName();
}

int main()
{
    PhoneBook myPhoneBook;

    std::cout << "Welcome to PhoneBook!\n\n";

    while(true)
    {
        std::cout << "\n1. Add Contact\n"
                     "2. Display Contacts\n"
                     "3. Search Contact\n"
                     "4. Delete Contact\n"
                     "5. Edit Contact\n"
                     "6. Exit\n\n";

        std::cout << "Enter your choice: ";
        int choice;

        std::cin >> choice;

        switch(choice)
        {
            case 1:
                myPhoneBook.addContact();
                break;
            case 2:
                myPhoneBook.displayContacts();
                break;
            case 3:
            {
                std::string name;
                std::cout << "Enter name to search: ";
                std::cin >> name;
                myPhoneBook.searchContact(name);
                break;
            }
            case 4:
            {
                std::string name;
                std::cout << "Enter name to delete: ";
                std::cin >> name;
                myPhoneBook.deleteContact(name);
                break;
            }
            case 5:
            {
                std::string name;
                std::cout << "Enter name to edit: ";
                std::cin >> name;
                myPhoneBook.editContact(name);
                break;
            }
            case 6:
                exit(0);

            default:
                std::cout << "Invalid Choice!\n\n";
        }
    }

}
