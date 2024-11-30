#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <iostream> // Include iostream for std::cout
#include "RBTree.h" // Include the RBTree header

class Book {
private:
    int id;
    std::string title;
    std::string author;
    std::string category;
    std::string subcategory;
    std::string format;
    double rating;
    double reviews;
    std::string url;
    RBTree& rbTree; // Reference to RBTree
    Node* root; // Root of the RBTree
public:
    Book(int id, const std::string &title, const std::string &_author, 
     const std::string &_category, const std::string &_subcategory, 
     const std::string &_format, double rating, double reviews,
     const std::string &_url, RBTree* _rbTree = nullptr)
    : id(id), title(title), author(_author), category(_category), 
      subcategory(_subcategory), format(_format), 
      rating(rating), reviews(reviews), url(_url), 
      rbTree(_rbTree ? *_rbTree : *(new RBTree())), 
      root(nullptr) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getCategory() const { return category; }
    std::string getSubcategory() const { return subcategory; }
    std::string getFormat() const { return format; }
    double getRating() const { return rating; }
    double getReviews() const { return reviews; }
    std::string getUrl() const { return url; }
    bool isFavorite = false;
    void display() const {
        std::cout << "ID: " << id << "\n"
                  << "Title: " << title << "\n"
                  << "Author: " << author << "\n"
                  << "Category: " << category << "\n"
                  << "Subcategory: " << subcategory << "\n"
                  << "Format: " << format << "\n"
                  << "Rating: " << rating << "\n"
                  << "Reviews: " << reviews << "\n"
                  << "URL: " << url << "\n\n";
    }

    static Book fromString(const std::string& str) {

        std::stringstream ss(str);

        std::string item;

        std::vector<std::string> parts;

        

        while (std::getline(ss, item, ',')) {

            parts.push_back(item);

        }

        

        if (parts.size() >= 9) {

            return Book(

                std::stoi(parts[0]),

                parts[1],

                parts[2],

                parts[3],

                parts[4],

                parts[5],

                std::stof(parts[6]),

                std::stof(parts[7]),

                parts[8]

            );

        }

        return Book(0, "", "", "", "", "", 0.0f, 0.0f, "");

    }
};

#endif // BOOK_H