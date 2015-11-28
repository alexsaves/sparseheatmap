{
  "targets": [
    {
      "target_name": "sparsematrix",
      "sources": [
        "nodeheatmap.cpp",
        "sparsearray.cpp",
        "sparsematrix.cpp",
        "sparsescroll.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}