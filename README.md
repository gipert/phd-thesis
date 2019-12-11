# phd-thesis

### Rules and conventions
- label names: `[fig|eq|chap|sec|...]:group:uniqueid`

### `standalone` template
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
