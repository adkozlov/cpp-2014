#include <cstddef>
#include <queue>
#include <map>
#include <algorithm>

#include "codec.h"

namespace
{
    struct node
    {
        char_t value;
        size_t frequency;
        node *left, *right;

        node(const char_t value, const size_t frequency)
            : value(value)
            , frequency(frequency)
            , left(NULL)
            , right(NULL)
        {
        }

        node(node *left, node *right)
            : value(0)
            , frequency(left->frequency + right->frequency)
            , left(left)
            , right(right)
        {
        }
    };

    struct node_comparison
    {
        bool operator()(const node *left, const node *right) const
        {
            return left->frequency >= right->frequency;
        };
    };
} // tree node

typedef std::priority_queue<node*, std::vector<node*>, node_comparison> node_queue;
typedef std::pair<const node*, size_t> node_level;
typedef std::queue<node_level> nodes_levels_queue;

namespace
{
    node* pop_top(node_queue &queue)
    {
        node *result = queue.top();
        queue.pop();

        return result;
    }

    void push_node_level(nodes_levels_queue &queue, const node *node, const size_t level)
    {
        queue.push(std::make_pair(node, level));
    }
} // queue utils

codec::codec(const size_t size, const size_t decoding_maps_size=0, const size_t min_code_size=0)
    : min_code_size(min_code_size)
{
    codes.resize(size);
    decoding_maps.resize(decoding_maps_size);
}

codec codec::build_by_frequencies(const std::vector<size_t>& frequencies)
{
    node_queue queue;
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        if (frequencies[i] > 0)
        {
            queue.push(new node(i, frequencies[i]));
        }
    }

    if (queue.empty())
    {
        return codec(0);
    }

    while (queue.size() != 1)
    {
        node* left = pop_top(queue);
        node* right = pop_top(queue);

        queue.push(new node(left, right));
    }

    nodes_levels_queue pair_queue;

    node* root = pop_top(queue);
    push_node_level(pair_queue, root, (root->left == NULL && root->right == NULL) ? 1 : 0);

    std::vector<char_t> lengths(TABLE_SIZE);
    while (!pair_queue.empty())
    {
        node_level e = pair_queue.front();
        pair_queue.pop();

        const node* node = e.first;
        size_t level = e.second;

        if (node->left != NULL)
        {
            push_node_level(pair_queue, node->left, level + 1);
        }
        if (node->right != NULL)
        {
            push_node_level(pair_queue, node->right, level + 1);
        }

        if (node->left == NULL && node->right == NULL)
        {
            lengths[node->value] = level;
        }

        delete node;
    }

    return build_by_code_lengths(lengths);
}

codec codec::build_by_code_lengths(const std::vector<char_t>& lengths)
{
    size_t min_code_size = *std::min_element(lengths.begin(), lengths.end());
    size_t max_code_size = *std::max_element(lengths.begin(), lengths.end());

    std::map<size_t, size_t> length_count;
    for (size_t i = 0; i < TABLE_SIZE; i++)
    {
        if (lengths[i] > 0)
        {
            length_count[lengths[i]]++;
        }
    }

    std::map<size_t, huffman_code> start_codes;
    start_codes[max_code_size] = huffman_code(max_code_size, 0);

    if (max_code_size > 0)
    {
        for (size_t i = max_code_size - 1; i > 0; i--)
        {
            huffman_code last = start_codes[i + 1];

            huffman_code result(last.size(), last.long_value() + length_count[i + 1]);
            result.shift_right();
            start_codes[i] = result;
        }
    }

    codec result(TABLE_SIZE, max_code_size + 1, min_code_size);
    for(size_t i = 0; i < TABLE_SIZE; i++)
    {
        if (lengths[i] > 0)
        {
            size_t length = lengths[i];

            huffman_code code = start_codes[length];
            result.codes[i] = code;
            result.decoding_maps[length][code.long_value()] = std::make_pair(code, (char_t) i);

            ++code;
            start_codes[length] = code;
        }
    }

    return result;
}

size_t codec::get_min_code_size() const
{
    return min_code_size;
}

void codec::encode_uint32(const uint32_t n, char_t buffer[UINT32_SIZE])
{
    for (size_t i = 0; i < UINT32_SIZE; i++)
    {
        buffer[i] = (n >> i * BYTE_SIZE);
    }
}

uint32_t codec::decode_uint32(const char_t buffer[UINT32_SIZE])
{
    uint32_t result = 0;

    for (int i = UINT32_SIZE - 1; i >= 0; i--)
    {
        uint32_t value = buffer[i];

        result |=  value << i * BYTE_SIZE;
    }
    return result;
}

bool codec::encode(const char_t symbol, huffman_code* result)
{
    if ((size_t) symbol >= codes.size())
    {
        return false;
    }

    if (codes[symbol].size() > 0)
    {
        *result = codes[symbol];

        return true;
    }
    else
    {
        return false;
    }
}

bool codec::decode(const huffman_code& code, char_t* c)
{
    decoding_map::iterator match = decoding_maps[code.size()].find(code.long_value());

    if (match == decoding_maps[code.size()].end())
    {
        return false;
    }
    else
    {
        *c = match->second.second;

        return true;
    }
}

void codec::copy_sizes(char_t lengths[TABLE_SIZE])
{
    if (!codes.empty())
    {
        for (size_t i = 0; i < TABLE_SIZE; i++)
        {
            lengths[i] = codes[i].size() > 0 ? codes[i].size() : 0;
        }
    }
}
