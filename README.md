# Electronic Parking Disc

## Terminal usage

`cd directoryname`: Change directory

`ls` tai `dir`: List the contents of a directory

## Git cheat sheet

`git clone url`: Creates a local working copy of the repository. This does only have to be done once per computer (unless the folder is removed).

`git status`: Shows all changes in the directory. If a file has been modified without being *staged* it is shown in read. If a file has been *staged* but not committed it appears as green.

`git checkout (-b) branchname`: Switches to another branch. When viewing the files on the computer they will appear as they do in that particular branch. To create a branch, add `-b`. Without the `-b`, it will change to an existing branch.

`git add filename`: *Stages* a file. This means that we have made changes to a file that we want to commit.

`git commit -m "nice descriptive message here"`: *Commits* the changes to the files that have been *staged* (added).

`git push`: *Pushes* (uploads) all the local changes to the main repository. Only committed changes will be pushed.

`git pull`: *Pull* (downloads) the latest version from the main repository.

## Git guidelines

To make changes to the code, first clone the repository to your computer using `git clone https://github.com/relex/cruncher.git`.

In Git, the main version of the project (*the master branch*) should only contain working code. When doing changes to the code, first create a new *branch*. Branches can be seen as a new copy of the code, changes to this branch will not affect the master branch. This way, developers can break things without it affecting the project as a whole. Creating a new branch is done by `git checkout -b branchnamehere` (to switch between existing branches, drop the `-b`).

After doing some changes to the code, the changes have to be *commited*. Typing `git status` will show you what files have been changed since the last commit. To commit a change, first *stage* the file (we tell Git what files we want to commit changes to) by typing `git add maybeASubdirectoryOrTwo/filenamehere`. After this, the changes can be commited using `git commit -m "Nice descriptive message here"`.

Local changes should regularly be *pushed* to the main repository, in case the computer blows up or something. This is done with `git push`. The branch will now be visible on Github as well.

When a change is deemed to be completed, the developer asks to merge the branch with the master branch using a *pull request*. This is done from the pull requests tab on the Github project page. Select both the branch and the master branch, add some comments if necessary. A teammate then looks through the code. If there is something that needs to be fixed, commit and push changes to the same branch as used before. They will appear automatically in the pull request.

Once the pull request is confirmed, the changes will be introduced to the master branch.
