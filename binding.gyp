{
  "targets": [
    {
      "target_name": "sparsematrix",
      "sources": [
        "nodeheatmap.cpp",
        "sparsematrix.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}