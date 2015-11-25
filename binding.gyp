{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "sparsematrix.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}