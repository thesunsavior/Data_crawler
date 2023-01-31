#include "news.h"

const char spec_char[] = "!?@#.$%^&*(),;{}[]~:\"";

void ParseNewsLine(string line, string &text)
{

    if (line.substr(0, 4) == "<pr>")
    {
        // get headers
        int until = line.find(":");
        string field = line.substr(4, until - 4);
        string value = line.substr(until + 2);

        if (field == "body")
        {
            text = value;
            return;
        }
    }
    else
        text += line;
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

void News::ImportFromString(string text)
{
    int mini = 999998;
    if (text.length() < mini)
        mini = text.length();
    strncpy(this->text, text.c_str(), mini);
    this->text[mini + 1] = '\0';
}

void News::Clear()
{
    // text = "";
    // empty for now
}

bool Doc::contains(string word)
{
    if (bag_of_words.count(word) == 1)
        return true;

    return false;
}

int Doc::get_value(string word)
{
    if (!contains(word))
        return 0;

    return bag_of_words[word];
}

void Doc::insert(string word)
{
    if (contains(word))
        bag_of_words[word]++;
    else
        bag_of_words[word] = 1;
}

int Doc::dot_product(Doc doc2)
{
    int sum = 0;
    for (auto iter = cbegin(bag_of_words); iter != cend(bag_of_words); ++iter)
        sum += get_value(iter->first) * doc2.get_value(iter->first);

    return sum;
}

// square euclid length of current doc words vector
double Doc::sq_euclid_length()
{
    double sum = 0.0;
    for (auto iter = cbegin(bag_of_words); iter != cend(bag_of_words); ++iter)
        sum += iter->second * iter->second;

    return sum;
}

// cosine distance of the currently consider doc with doc2
double Doc::cos_similarity(Doc doc2)
{
    // sqrt here to get small marginal error
    return dot_product(doc2) / sqrt(sq_euclid_length() * doc2.sq_euclid_length());
}

void Doc::ExtractTextToBOW(string text)
{
    this->raw_text = text;

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
            for (int j = 0; j < 22; j++)
                if (text[i] == spec_char[j])
                    continue;

            temp += text[i];
        }
    }

    if (temp != " ")
        this->insert(temp);
}

string Doc::GetRawText()
{
    return this->raw_text;
}