#include "news.h"

void News::ParseNewsLine(string line)
{

    if (line.substr(0, 4) == "<pr>")
    {
        // get headers
        int until = line.find(":");
        string field = line.substr(4, until - 4);
        string value = line.substr(until + 2);

        if (field == "url")
        {
            this->url = value;
            return;
        }
        else if (field == "body")
        {
            this->text = value;
            return;
        }
        else if (field == "NLP_keywords")
        {
            this->NLP_keywords = seperator(value, ',');
            return;
        }
        else if (field == "keywords")
        {
            this->keywords = seperator(value, ',');
            return;
        }
        else if (field == "Date")
        {
            this->publish_date = value;
        }
    }
    else
        this->text += line;
}

void News::Clear()
{
    this->url = "";
    this->text = "";
    this->publish_date = "";
    this->keywords.clear();
    this->NLP_keywords.clear();
}

void ExecNewsParsing(string source_file_without_ext, string result_file_without_ext, string mode)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        cout << ">>>>>>>>>>>>> Executing child process news parser <<<<<<<<<<<<<" << endl;
        string cmd = "python3 ../news_parser.py " + source_file_without_ext + ".txt " + result_file_without_ext + ".txt" + " " + mode;
        ShellExec(cmd.c_str());
        exit(0);
    }
    else if (pid > 0)
    {
        int status = 0;
        wait(&status);
    }
    else
    {
        cout << "!!!!!!!!!!  News parsing failed, Error on creating child process !!!!!!!!!!!! " << endl;
        exit(0);
    }
    cout << "=================== Child process done ====================" << endl;
}

bool News::contains(string word)
{
    if (bag_of_words.count(word) == 1)
        return true;

    return false;
}

int News::get_value(string word)
{
    if (!contains(word))
        return 0;

    return bag_of_words[word];
}

void News::insert(string word)
{
    if (contains(word))
        bag_of_words[word]++;
    else
        bag_of_words[word] = 1;
}

int News::dot_product(News &doc2)
{
    int sum = 0;
    for (auto iter = cbegin(bag_of_words); iter != cend(bag_of_words); ++iter)
        sum += get_value(iter->first) * doc2.get_value(iter->first);

    return sum;
}

// square euclid length of current doc words vector
double News::sq_euclid_length()
{
    double sum = 0.0;
    for (auto iter = cbegin(bag_of_words); iter != cend(bag_of_words); ++iter)
        sum += iter->second * iter->second;

    return sum;
}

// cosine distance of the currently consider doc with doc2
double News::cos_similarity(News &doc2)
{
    // sqrt here to get small marginal error
    return dot_product(doc2) / sqrt(sq_euclid_length() * doc2.sq_euclid_length());
}

void News::ExtractTextToBOW()
{
    string temp;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == ' ')
        {
            this->insert(temp);
            temp = "";
        }
        else
        {
            // remove special character
            if (isalpha(text[i]) || isdigit(text[i]))
                temp += text[i];
        }
    }

    if (temp != " ")
        this->insert(temp);
}