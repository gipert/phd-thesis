# phd-thesis

<p align="center">
  <img src="https://github.com/gipert/phd-thesis/blob/master/.github/progress.png?raw=true" width="100%" alt="Progress Tracker">
</p>

Requirements: GNU Make, XeLaTeX, Asymptote. Run `make` in the `src` directory to compile the project.

### Rules and conventions
- label names: `[fig|eq|chap|sec|...]:group:uniqueid`
- Use `\cref` instead of `\ref`

### `standalone` Garamond template
```tex
%! TEX program = xelatex
\documentclass[tikz]{standalone}
\usepackage[lining]{ebgaramond}%[StylisticSet={7,9}]
\usepackage[math-style=ISO, bold-style=ISO]{unicode-math}
\setmathfont{Garamond-Math.otf}
\begin{document}
    % ...
\end{document}
```
