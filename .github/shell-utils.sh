update_progress_plot() {
    \cd `git rev-parse --show-toplevel`
    echo `compose_line` >> .github/wc.dat
    \cd - > /dev/null

    xelatex -shell-escape progress.tex
}

generate_all() {
    \cd `git rev-parse --show-toplevel`
    for commit in `git rev-list --all`; do
        rm .github/wc.dat
        echo `compose_line_from_commit $commit` >> .github/wc.dat
    done
    \cd - > /dev/null
}

compose_line() {
    echo `date +'%Y-%m-%d %H:%M:%S'`, \
         `cat $(find src      | grep -E '.*\.(bib|tex|asy|C)') | wc -w`, \
         `cat $(find src/chap | grep -E '.*\.(tex)')           | wc -w`, \
         `cat $(find src/img  | grep -E '.*\.(tex|asy|C)')     | wc -w`, \
         `cat $(find src/bib  | grep -E '.*\.(bib)')           | wc -w`
}

compose_line_from_commit() {
    ts=`git --no-pager log -n 1 --pretty=%at $1`
    echo `date -d @$ts +'%Y-%m-%d %H:%M:%S'`, \
         `git archive $1 -- $(git ls-tree -r --name-only $1             | grep -E '.*\.(tex|bib|asy|C)') | tar -x -O | wc -w`, \
         `git archive $1 -- $(git ls-tree -r --name-only $1 -- src/chap | grep -E '.*\.tex')             | tar -x -O | wc -w`, \
         `git archive $1 -- $(git ls-tree -r --name-only $1 -- src/img  | grep -E '.*\.(tex|asy|C)')     | tar -x -O | wc -w`, \
         `git archive $1 -- $(git ls-tree -r --name-only $1 -- src/bib  | grep -E '.*\.bib')             | tar -x -O | wc -w`
}
