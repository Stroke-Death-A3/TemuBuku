#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <iostream>

class Book
{
private:
    int id;
    std::string title;
    std::string author;
    std::string category;
    std::string subcategory;
    std::string format;
    std::string url;
    double rating;
    double reviews;
    

public:
    Book(int _id = 0, const std::string &_title = "", const std::string &_author = "",
         const std::string &_category = "", const std::string &_subcategory = "",
         const std::string &_format = "", double _rating = 0.0,
         double _reviews = 0.0, const std::string &_url = "")
        : id(_id), title(_title), author(_author), category(_category),
          subcategory(_subcategory), format(_format),
          rating(_rating), reviews(_reviews), url(_url) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getCategory() const { return category; }
    std::string getSubcategory() const { return subcategory; }
    std::string getFormat() const { return format; }
    double getRating() const { return rating; }
    double getReviews() const { return reviews; }
    std::string getUrl() const { return url; }
    void display() const
    {
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
};
#endif