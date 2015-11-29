{
  "targets": [
    {
      "target_name": "sparsematrix",
      "sources": [
        "cpp/nodeheatmap.cpp",
        "cpp/sparsearray.cpp",
        "cpp/sparsematrix.cpp",
        "cpp/sparsescroll.cpp",
        "cpp/colormodel.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}