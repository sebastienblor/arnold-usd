# Contributing to this repository

## Prepare your local git environment

- [Download and install](https://git-scm.com/download) the latest available **git**. If you are on CentOS or RHEL, where older versions are shipped, you can always build the latest git from the source code.
- [Download and install](https://github.com/git-lfs/git-lfs/releases/) **git-lfs** ([Git Large File Storage or LFS](https://git-lfs.github.com/)). This is transparently used by git commands to store and manage large binary files in a separate place, so that the git repository keeps being lightweight. Ideally, you won't have to deal with git-lfs commands unless you want to track a new binary file extension or you are doing some git-lfs surgery. The git-lfs installation should always conclude by manually running the following command (only once), before cloning any repository with LFS files. Regardless of if git-lfs was installed system-wide, every user has to execute the command the very first time (and only once) (docs: [_git-lfs_](https://github.com/git-lfs/git-lfs/tree/main/docs))
```
git lfs install
```
- Before trying to fork and clone any repository available in `https://github.com/autodesk-external`, execute the following command. That will instruct git to dynamically replace any appearance of the non-reachable internal URLs `https://git.autodesk.com/arnold` with the "public" URL `https://github.com/autodesk-external` that you have access to (docs: [_git-config-insteadof_](https://git-scm.com/docs/git-config#Documentation/git-config.txt-urlltbasegtinsteadOf))

```
git config --global url."https://github.com/autodesk-external".insteadOf https://git.autodesk.com/arnold
```
- Some of the submodules in the project can't be accessed externally. In order to ignore them, and don't get any git error, you will have to disable them as follows after cloning 
(docs: [_git-config-submodule.\<name\>.update_](https://git-scm.com/docs/git-config#Documentation/git-config.txt-submoduleltnamegtupdate))

```
cd arnold-mtoa
git config --local submodule."core/src/shaders/c4dtoa/noises".update none
```

## Fork the upstream repository

Go to [`https://github.com/autodesk-external/arnold-mtoa`](https://github.com/autodesk-external/arnold-mtoa)

In the top-right corner of the page, click **Fork**. That will create `https://github.com/USERNAME/arnold-mtoa` in your user space.
NOTE: _This fork is actually a snapshot at the moment you fork the upstream repository, and it won't be automatically synchronized with later changes in the upstream_.

## Clone your fork

Instead of using the password of your GitHub account for cloning, fetching, pulling, pushing, etc., it's recommended to [create a Personal Access Token](https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token), enabling the scope _repo_, and use it as if it was your password. In order to cache your "new" credentials, there are built-in git credential helpers for macOS and Windows ([more info on Git Credential Storage](https://git-scm.com/book/en/v2/Git-Tools-Credential-Storage)). For Linux you can use [libsecret](https://developer.gnome.org/libsecret/) (Debian/Ubuntu) or [lastpass-cli](https://github.com/lastpass/lastpass-cli) (Red Hat/CentOS). After that, you can clone your fork with the following command. (docs: [_git-clone_](https://git-scm.com/docs/git-clone), [_git-submodule_](https://git-scm.com/docs/git-submodule), [_git-config-credential_](https://git-scm.com/docs/gitcredentials))

```
git clone https://github.com/USERNAME/arnold-mtoa
cd arnold-mtoa
git submodule sync --recursive
git submodule update --init --recursive
```

Add a new remote for the upstream called `upstream`, so that you can get updates from the "central" repository every time you execute `git fetch upstream` or `git fetch --all`. (docs: [_git-remote_](https://git-scm.com/docs/git-remote), [_git-fetch_](https://git-scm.com/docs/git-fetch))

```
git remote add upstream https://github.com/autodesk-external/arnold-mtoa
git fetch --all
```

At this point you have two remotes configured (`origin` being your fork, the default remote when cloning). You can list them by running `git remote -v`. Anytime you execute `git fetch --all` you'll fetch all the new content in both remotes (origin and upstream). If you just want to fetch only the new content from one of the remotes, you can make so with `git fetch origin` or `git fetch upstream`. A plain `git fetch` will fetch only from origin (since it's the default remote).

## Contributing with Pull Requests

Create a new `BRANCH` in your fork for this contribution based off of the current upstream's master branch. If there is a JIRA ticket for tracking your contribution, please use its name (eg. `ARNOLD-1234`) as the branch name. (docs: [_git-fetch_](https://git-scm.com/docs/git-fetch), [_git-checkout_](https://git-scm.com/docs/git-checkout), [_git-push_](https://git-scm.com/docs/git-push))

```
git fetch --all
git checkout -b BRANCH --no-track upstream/master
git push -u origin BRANCH
```

Push commits to the new branch. Again, use the JIRA ticket name in the commit message, if available, with the following format. (docs: [_git-add_](https://git-scm.com/docs/git-add), [_git-commit_](https://git-scm.com/docs/git-commit), [_git-push_](https://git-scm.com/docs/git-push))

```
... add/modify files ...
git add <new and modified files>
git commit -m "MTOA-1234 - The commit title"
git push
```

At any moment after creating your `BRANCH` you can go to the upstream repository at [`https://github.com/autodesk-external/arnold-mtoa`](https://github.com/autodesk-external/arnold-mtoa), and [create a Pull Request](https://github.com/autodesk-external/arnold-mtoa/compare) (comparing across forks) with the upstream master branch as the base branch, and your fork `BRANCH` as the head branch. If your branch is still WIP or not yet prepared for merging, you can create the PR as a "draft" and that will avoid merging it until you enable it as "ready for review".

Before pushing any change, make sure that Arnold [builds](BUILDING.md) and passes the testsuite.

An Arnold team member will review, approve and merge the PR, which will be transferred to the internal repository for further checks. Once it's internally approved, the contribution will be synchronized back in [`https://github.com/autodesk-external/arnold-mtoa`](https://github.com/autodesk-external/arnold-mtoa).
