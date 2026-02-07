#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Question
{
    int topic;
    int difficulty;
    string question;
    string options[4];
    int correct;
};

int loadQuestions(const string &filename, Question q[], int topic, int diff)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Unable to open file!\n";
        return 0;
    }

    string line;
    int count = 0;
    int lineNo = 0;

    while (getline(file, line))
    {
        lineNo++;

        // Skip empty or junk lines
        if (line.empty())
            continue;

        stringstream ss(line);
        string temp;

        Question tempQ;

        try
        {
            // topic
            if (!getline(ss, temp, '|'))
                continue;
            tempQ.topic = stoi(temp);

            // difficulty
            if (!getline(ss, temp, '|'))
                continue;
            tempQ.difficulty = stoi(temp);

            // question
            if (!getline(ss, tempQ.question, '|'))
                continue;

            // options
            for (int i = 0; i < 4; i++)
            {
                if (!getline(ss, tempQ.options[i], '|'))
                    throw invalid_argument("Missing option");
            }

            // correct answer
            if (!getline(ss, temp, '|'))
                throw invalid_argument("Missing answer");

            tempQ.correct = stoi(temp);
        }
        catch (...)
        {
            // Bad line → skip safely
            continue;
        }

        if (tempQ.topic == topic && tempQ.difficulty == diff)
        {
            q[count++] = tempQ;
        }
    }

    file.close();
    return count;
}

void shuffle(Question q[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int r = rand() % n;
        swap(q[i], q[r]);
    }
}

void startQuiz(Question q[], int total, int questionsToAsk)
{

    float score = 0;

    cout << "\n---------------- QUIZ STARTED ----------------\n";

    for (int i = 0; i < questionsToAsk; i++)
    {
        cout << "\nQ" << i + 1 << ": " << q[i].question << "\n";
        for (int j = 0; j < 4; j++)
            cout << j + 1 << ". " << q[i].options[j] << "\n";

        int ans;
        cout << "Your answer (1-4, 0 to skip): ";
        cin >> ans;

        if (ans == 0)
        {
            cout << "Skipped! Correct answer is: "
                 << q[i].correct << ". "
                 << q[i].options[q[i].correct - 1] << "\n";
            continue;
        }

        if (ans == q[i].correct)
        {
            cout << "Correct!\n";
            score += 1;
        }
        else
        {
            cout << "Wrong! Correct answer: " << q[i].correct << "\n";
            score -= 0.25;
        }
    }

    cout << "\nFinal Score: " << score << " / " << questionsToAsk << "\n";
}

int main()
{
    srand(time(0));

    Question questions[500];
    int topic, diff;

    cout << "\n       Welcome to the Quiz Application!!!    \n";

    cout << "\nSelect Topic:\n";
    cout << "1. C++\n2. Python\n3. JavaScript\n4. HTML & CSS\n";
    cout << "5. Maths\n6. Tech\n7. GK\n8. DSA\n";
    cout << "Enter topic: ";
    cin >> topic;

    cout << "\nSelect Difficulty:\n1. Easy\n2. Medium\n3. Hard\n";
    cout << "Enter difficulty: ";
    cin >> diff;

    string filename;
    switch (topic)
    {
    case 1:
        filename = "questions/cpp.txt";
        break;
    case 2:
        filename = "questions/python.txt";
        break;
    case 3:
        filename = "questions/javascript.txt";
        break;
    case 4:
        filename = "questions/html_css.txt";
        break;
    case 5:
        filename = "questions/maths.txt";
        break;
    case 6:
        filename = "questions/tech.txt";
        break;
    case 7:
        filename = "questions/gk.txt";
        break;
    case 8:
        filename = "questions/dsa.txt";
        break;
    default:
        cout << "Invalid topic!\n";
        return 0;
    }

    int total = loadQuestions(filename, questions, topic, diff);

    if (total == 0)
    {
        cout << "No questions found!\n";
        return 0;
    }

    int userLimit;
    cout << "\nHow many questions do you want to attempt? ";
    cin >> userLimit;

    // Validation
    if (userLimit <= 0)
    {
        cout << "Invalid number of questions!\n";
        return 0;
    }

    if (userLimit > total)
    {
        cout << "Only " << total << " questions available. Adjusting limit.\n";
        userLimit = total;
    }

    shuffle(questions, total);
    startQuiz(questions, total, userLimit);

    cout << "\n  Thank u for using our Quiz Application    \n";
    cout << "\n         Have a Nice Day!!!    \n";

    return 0;
}
