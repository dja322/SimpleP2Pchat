# SimpleP2Pchat
A basic P2P chatting CLI app that uses simple RSA encryption
This is for testing how to go about making a simple peer to peer chat app

Encryption using OpenSSL bignum is intended to be added in a separate version.

# Features
- Connect to other person running the program to chant
- List contacts
- Basic Encryption(Use OpenSSL version for more secure version)

# Prerequisites
C/C++ compiler (e.g., GCC, Clang, or MSVC).

Compatible OS for console execution.

# How to use
Run the makefile to produce the p2pchat.exe executable

You will be given menus to navigate program features

1. Connect to peer – Start a network session with another user.
   You will either have to host as a server or connect to a hot using their ip address
   Once you are connected your username will be sent over and you will be able to chat

2. Contacts – View and manage saved contacts.
   Manage list of contacts, listing, adding, or removing them

3. Settings – Configure user preferences.
   Basic settings feature, only used setting is username

4. Exit – Close the application.

Planned Features:
- Create version using OpenSSL for good RSA encryption
- Encrypt local data using a user set password
- Ability to save messages
- Optimizations
