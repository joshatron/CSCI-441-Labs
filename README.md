# Labs

This repository is where all your lab assignments and code will be provided. You will fork this repository into your own private Bitbucket repository, where you will make your additions to the code base. The reason for Bitbucket is public repositories are free on GitHub, whereas private ones are free on Bitbucket. I will be adding assignments to this repository throughout the semester so you will need to follow the instructions below in order to keep your repository up-to-date, while also being able to push changes up to your own personal repository.

## Sign up for Bitbucket

First, you'll need a Bitbucket account, so [sign up here](https://bitbucket.org/account/signup/). 

## Fork the repository

You'll need to fork this repository into your own private repository. In the left hand menu you can click the 'Fork' button to start the process. Change the name to CSCI 441 Labs to be more descriptive and add a description if you like. Check 'This is a private repository', so no one else has access to it (we'll add your lab teammates in a bit). Click the 'Fork repository' button.

## Share the repository

Since you're encouraged to work in teams for the labs, you'll probably want to share the repository with your teammates. You can also share the repository with me to make it easier for me to help you if the need arrises. In the upper right, there is usually an 'Invite users to this repo' box with a 'Send invitation' button. Click the button to add users (my username is jallwine). If the 'Invite users to this repo' box isn't there, you can also add users through the 'Settings' menu on the left hand side. Click 'Settings', then 'Account management', where you can add users.

## Install Git

Now we need to make sure git is installed. There are a number
of git GUI clients, including Bitbucket's [SourceTree](https://www.atlassian.com/software/sourcetree/overview),
or [others](http://git-scm.com/downloads/guis).
I'll leave you to figure out your GUI of choice on your own if you like. 
I always stick to the command line when I can, so that's what I'll be showing
you how to do. In a terminal run the following:

    git --version 

If you get an error you may need to [download and install git](http://git-scm.com/downloads). 
If you've never used git before you might want to [read up on the docs](http://git-scm.com/doc), 
or go through the basics with [Try Git](try.github.com).

## Clone your repository


To download your repository so it's ready for modifications, we need to clone it.
Once git is installed, run the following command with <repo url> replaced with
the url to your repository (shown in the upper right of the Overview page on Bitbucket).
It'll look something like https://jallwine@bitbucket.org/jallwine/csci-441-labs.git.

    git clone <repo url>

It'll ask for your Bitbucket password. You'll have to enter your password every time you
want to push changes up to Bitbucket or pull changes down, unless you set up SSH keys (go to
Manage Account in the top right most menu, then under security is SSH keys).

## How to pull the latest assignments

I'll be adding assignments here throughout the semester so you'll need to add
this repository as a remote that you can pull changes from. Make sure you cd into
your repository, something like (depending on what you named it):

    cd csci-441-labs

Then let's list what remotes we currently have:

    git remote -v

You should see two entries for your Bitbucket repository, one (fetch) and one (push). When
you use `git push` or `git pull`, by default you'll push or pull to/from that url. We want to
add another entry that we can explicitly pull from to merge in assignments as I add them. To
do that, run the following:

    git remote add csc441 https://bitbucket.org/msucsc441spring2016/labs

Now if you run the `git remote -v` command you'll see two additional entries. You won't have permission
to push to this one, but you will be able to pull new assignments by running the following:

    git pull csc441 master

## Install Qt

All assignments and labs will be written in C++ using OpenGL and Qt. Qt is a cross platform
UI framework. There are both commercial and open source licenses for Qt.  We'll be using the 
open source version, which can be downloaded [here](http://www.qt.io/download-open-source/).
Follow the instructions for installation on your platform.

## Build Lab 1

Once you have Qt installed, try compiling and running the provided lab 1 code. Qt projects
are organized using project files (.pro files). The qt command `qmake` can generate a Makefile
(or an Xcode project, or a Visual Studio project). I like to stick to using a Makefile so I can
stay in a terminal:  

    cd lab1
    qmake
    make
    ./lab1

    Windows users:
    cd lab1
    qmake
    mingw32-make
    cd release
    lab1.exe

It should print "Output triangle.jpg" to the terminal and triangle.jpg should be a blank, black image (it's
your job to make it output a triangle). On the lab computers (in EPS 254, and others), multiple versions of qt are 
installed on Fedora. To use the right version of qmake use `qmake-qt5`.

## How To Save Your Work

Once you start making changes to your code, you'll want to commit those changes and push them back up
to the server. You should do this often to keep a good history of what changes you made and why. It also ensures your files are safely stored on the server. 

For every file you've changed or created, you'll want to run `git add <file name>`. This
stages the file for commit. You can think of staging a file as flagging that you want to save it in your
next commit. Once you've staged every file that you want to commit type:

    git commit -m "a short description of what you're committing"

It can be handy to see what files have been changed. To see what changes have been made you can 
use `git status`. `git status` will show you which files you've modified, which files are new 
and which files have been staged for commit. One thing to be careful of is to not commit files that are generated by the build process (i.e. the Makefile, the final executable, .o files, etc.). I've included a .gitignore file in the repository that will ignore most files that will be generated by your build processes, but others may be generated as well that I'm not aware of (especially if you're running your code on Windows).

You can commit as many times as you want locally. When you're ready to submit all your commits up to the
server, where I can see them, run:

    git push origin master

If you've cloned your repository in more than one location (such as on a lab computer and on your own laptop) and have made changes to one of them and pushed those changes up to the server, you can use `git pull` from the other location to bring it up to date.

## Submitting your work

For the labs, you may be able to just demo your work to me in class or in office hours before the deadline, but this may be difficult as there are many students that need to get checked off. An alternative would be to submit a pull request prior to the start of class on the day of the deadline. After committing your changes, and pushing them up to the server, you can submit a pull request by clicking 'Create pull request' in the left hand menu. Fill in a descriptive title and description (including which lab number and the names of your partners is a good place to start). Then click 'Create pull request'.

## More Git Basics

Git is a distributed version control system (VCS). When you cloned the repository above, you downloaded
every change that has ever been made to the repository. You can see each individual change by running the 
following:

    git log

You'll see a list of commits, which are changes to a set of files in your repository. Each commit looks
something like this (this is the first commit to your repository, made by me):

    commit 64b2f40ee1bfe685a301bbf912aeb87b46e5ca28
    Author: John Allwine <jallwine86@gmail.com>
    Date:   Sat Dec 27 16:49:04 2014 -0700

        added lab1

The first line shows the hash value for the commit. If you ever want to revert back to a specific commit
you can use `git checkout <hash value>`. In your repository, you can type the following to get to the very
first change ever made:

    git checkout 64b2f40ee1bfe685a301bbf912aeb87b46e5ca28

To get back to the lastest, you can run:

    git checkout master

For a more fully featured git cheat sheet go [here](https://training.github.com/kit/downloads/github-git-cheat-sheet.pdf).
The official git documentation is [here](http://git-scm.com/doc).