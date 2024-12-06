#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

int check_depth(int x)
{
    int depth = 0;
    while (x != 0)
    {
        x >>= 1;
        depth += 1;
    }
    return depth;
}

/*
===========================
Rotated Form
===========================
*/

void print_rotatedForm(vector<char> v)
{
    size_t max_depth = check_depth(v.size());
    size_t size1 = 1 << max_depth, now_depth = 1, now_pos = 1;
    string buffer[size1 + 1];

    for (size_t i = 0; i < v.size(); i += 1)
    {
        if (i == 0 || now_depth != static_cast<size_t>(check_depth(i + 1)))
        {
            now_depth = check_depth(i + 1);
            if (i != 0)
            {
                while (now_pos != size1)
                    buffer[now_pos++].push_back(' ');
                for (size_t j = 1; j <= size1; j += 1)
                    buffer[j].push_back(' ');
                now_pos = 1;
            }

            for (int k = 1; k <= (1 << (max_depth - now_depth)) - 1; k += 1)
            {
                buffer[now_pos++].push_back(' ');
            }
        }

        buffer[now_pos++] += v[i]; // append
        if (i + 2 != size_t(1 << now_depth))
        {
            for (int k = 1; k <= (1 << (max_depth - now_depth + 1)) - 1; k += 1)
            {
                buffer[now_pos++].push_back(' ');
            }
        }
    }

    for (int i = 1 << max_depth; i >= 1; i--)
    {
        if (!all_of(buffer[i].begin(), buffer[i].end(), [](char c)
                    { return std::isspace(static_cast<unsigned char>(c)); })) // 귀찮은데 람다 박아도 되죠?
            cout << buffer[i] << endl;
    }
}

/*
===========================
Unrotated Form
===========================
*/

void print_unrotatedForm(vector<char> v)
{
    size_t max_depth = check_depth(v.size());

    int now_depth = 1;
    for (size_t i = 0; i < v.size(); i += 1)
    {
        if (i == 0 || now_depth != static_cast<size_t>(check_depth(i + 1)))
        {
            now_depth = check_depth(i + 1);
            if (i != 0)
                cout << '\n';
            for (int k = 1; k <= (1 << (max_depth - now_depth)) - 1; k += 1)
                cout << ' ';
        }

        cout << v[i];
        for (int k = 1; k <= (1 << (max_depth - now_depth + 1)) - 1; k += 1)
            cout << ' ';
    }
}

/*
===========================
H-Tree Form
===========================
*/

static int V[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}}; //(y,x)
vector<vector<char>> H_map;

int size_sigma(int x)
{
    if (x < 0)
        return 0;
    return (1 << (x / 2)) + size_sigma(x - 2);
}

void make_HTree(vector<char> v, int x, int y, int margin, int index, int up, int down, int right, int left)
{
    int cursor = index - 1;
    if (cursor > v.size())
        return;

    H_map[y][x] = v[cursor];
    if (2 * index <= v.size()) // draw left
    {
        H_map[y + margin * V[left][0]][x + margin * V[left][1]] = v[2 * index - 1];
        make_HTree(v,
                   x + margin * (V[left][1] + V[up][1]), // next x position
                   y + margin * (V[left][0] + V[up][0]), // next y position
                   margin >> 1,                          // next margin
                   index * 4,                            // next index
                   down, up, left, right);
        make_HTree(v,
                   x + margin * (V[left][1] + V[down][1]), // next x position
                   y + margin * (V[left][0] + V[down][0]), // next y position
                   margin - 2,                             // next margin
                   4 * index + 1,                          // next index
                   up, down, right, left);
    }
    if (2 * index + 1 <= v.size()) // draw right
    {
        H_map[y + margin * V[right][0]][x + margin * V[right][1]] = v[2 * index];
        make_HTree(v,
                   x + margin * (V[right][1] + V[down][1]), // next x position
                   y + margin * (V[right][0] + V[down][0]), // next y position
                   margin >> 1,                             // next margin
                   4 * index + 2,                           // next index
                   up, down, right, left);
        make_HTree(v,
                   x + margin * (V[right][1] + V[up][1]), // next x position
                   y + margin * (V[right][0] + V[up][0]), // next y position
                   margin - 2,                            // next margin
                   4 * index + 3,                         // next index
                   down, up, left, right);
    }
}

void print_HTree(vector<char> v)
{
    size_t max_depth = check_depth(v.size());
    size_t size;

    if (max_depth == 1)
    {
        size = 1;
    }
    else
    {
        size = size_sigma(max_depth);
    }

    for (size_t i = 0; i < size; i++)
    {
        H_map.push_back(vector<char>());
        for (size_t j = 0; j < size; j++)
        {
            H_map[i].push_back(32); // fill out with blank
        }
    }

    int x = size >> 1; // x-init : center
    int y = size >> 1; // y-init : center

    /*
    15x15 -> 5
    7x7 -> 3
    3x3 -> 1
    */
    int margin = ceil(size / 3);

    make_HTree(v, x, y, margin, 1, 1, 0, 2, 3);

    string buf[size];

    for (size_t i = 0; i < size; i++)
    {
        string line;
        for (size_t j = 0; j < size; j++)
        {
            if (H_map[i][j] == 32)
                line.push_back(' ');
            else
                line.push_back(H_map[i][j]);
        }
        buf[i] = line;
    }

    for (size_t i = 0; i < size; i++)
    {
        if (!all_of(buf[i].begin(), buf[i].end(), [](char c)
                    { return std::isspace(static_cast<unsigned char>(c)); })) // 귀찮은데 람다 박아도 되죠?
            cout << buf[i] << endl;
    }
}

int main()
{
    string input;
    vector<char> data;

    make_heap(data.begin(), data.end());

    for (int i = 0; i < 100; i++)
    {
        cin >> input;
        if (input[0] == 'i')
        {
            data.push_back(input[1]);
            push_heap(data.begin(), data.end());
        }
        if (input[0] == 'd')
        {
            pop_heap(data.begin(), data.end());
            data.pop_back();
        }
        if (input[0] == 'e')
        {
            break;
        }
        input.clear();
    }

    cout << endl;
    cout << "[Rotated form]" << endl;
    print_rotatedForm(data);
    cout << endl;
    cout << "[Unrotated form]" << endl;
    print_unrotatedForm(data);
    cout << endl;
    cout << endl;
    cout << "[H-tree form]" << endl;
    print_HTree(data);
    cout << endl;

    return 0;
}
