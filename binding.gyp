{
  "targets": [
    {
      "target_name": "sparsematrix",
      "sources": [
        "nodeheatmap.cpp",
        "sparsearray.cpp",
        "sparsematrix.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}