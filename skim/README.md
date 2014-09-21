## SUSY Skimmer

This exists to turn D3PDs into skims used in the analysis.

### General things that should be improved

This thing needs a bit of a refactor:

 - The `SusyBuffer` should be broken into two parts:
  + A `BranchBuffer` part that handles passing through by name and has
    a `set` method so that outside variables can be used by other
    routines.
  + A `SusyBuffer` part, which has all the specific stuff.
