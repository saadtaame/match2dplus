#include <cstdio>
#include <cstring>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

const int N = 5000;
const int M = N;
int n, m;

string haystack[N], needle[M];
int A[N][N]; /* filled by successive calls to match */
int p[M]; /* pattern to search for in columns of A */

struct Node
{   Node *a[256]; /* alphabet */
    Node *suff; /* pointer to node with prefix = longest proper suffix of this node */
    int flag; /* does this node correspond to a pattern? */
    vector<char> out; /* characters out of this node */

    Node()
    {   memset(a, 0, sizeof(a));
        suff = 0;
        flag = -1;
    }
};

void insert(Node *x, string s)
{   static int id = 0;
    static int p_size = 0;

    /* follow edges and make new ones if they don't exist */
    for(int i = 0; i < s.size(); i++)
    {   char c = s[i];
        if(x->a[c] == 0)
        {   x->a[c] = new Node;
            x->out.push_back(c);
        }
        x = x->a[c];
    }

    /* new pattern */
    if(x->flag == -1)
    {   x->flag = id++;
    }

    /* update pattern */
    p[p_size++] = x->flag;
}

/* find node whose label (path from root) is equal to x's longest proper suffix */
Node *suffix(Node *x, int c)
{   while(x->a[c] == 0)
    {   x = x->suff;
    }
    return x->a[c];
}

/* build the Aho-Corasick automaton */
Node *mk_automaton(void)
{   Node *trie = new Node;
    for(int i = 0; i < m; i++)
    {   insert(trie, needle[i]);
    }

    queue<Node*> q;

    /* level 1 */
    for(int i = 0; i < 256; i++)
    {   if(trie->a[i])
        {   trie->a[i]->suff = trie;
            q.push(trie->a[i]);
        }
        else trie->a[i] = trie;
    }

    /* level > 1 */
    while(q.empty() == false)
    {   Node *x = q.front(); q.pop();
        for(int i = 0; i < x->out.size(); i++)
        {   char c = x->out[i];
            if(x->a[c] == 0) continue;
            x->a[c]->suff = suffix(x->suff, c);
            q.push(x->a[c]);
        }
    }

    return trie;
}

/* search for patterns in haystack[j] */
void match(Node *x, int j)
{   for(int i = 0; i < n; i++)
    {   x = suffix(x, haystack[j][i]);
        if(x->flag != -1)
        {   A[j][i-m+1] = x->flag;
        }
    }
}

int match2d(Node *x)
{   int matches = 0;
    static int z[M+N];
    static int z_str[M+N+1];

    /* init */
    memset(A, -1, sizeof(A));

    /* fill the A matrix */
    for(int i = 0; i < n; i++)
    {   match(x, i);
    }

    /* build string for z algorithm */
    z_str[n+m] = -2; /* acts like `\0` for strings */
    for(int i = 0; i < m; i++)
    {   z_str[i] = p[i];
    }

    for(int i = 0; i < n; i++)
    {   /* search for pattern in column i */
        for(int j = 0; j < n; j++)
        {   z_str[j + m] = A[j][i];
        }

        /* run z algorithm */
        int l, r;
        l = r = 0;
        z[0] = n + m;
        for(int j = 1; j < n + m; j++)
        {   if(j > r)
            {   l = r = j;
                while(z_str[r] == z_str[r - l]) r++;
                z[j] = r - l;
                r--;
            }
            else
            {   if(z[j - l] < r - j + 1)
                {   z[j] = z[j - l];
                }
                else
                {   l = j;
                    while(z_str[r] == z_str[r - l]) r++;
                    z[j] = r - l;
                    r--;
                }
            }
        }

        /* locate matches */
        for(int j = m; j < n + m; j++)
        {   if(z[j] >= m)
            {   printf("match at (%d,%d)\n", j - m, i);
                matches++;
            }
        }
    }

    return matches;
}

int main(void)
{   cin >> n >> m;
    for(int i = 0; i < n; i++)
    {   cin >> haystack[i];
    }
    for(int i = 0; i < m; i++)
    {   cin >> needle[i];
    }

    Node *trie = mk_automaton();
    match2d(trie);

    return 0;
}
