# Analysis Code

Analysis is a funny name, this is the code that makes histograms out
of distilled ntuples.

### To do list

Some refactoring would be good:

 - The functions that add `Window`s to the n-1 plots should be moved into
   the `Selection` classes. This means `ISelection` would have a method
   like `ISelection::add_windows(...)`.
