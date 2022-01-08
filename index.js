
const {program_init, program_update, program_preview, program_save} = require('./build/Release/emit_from_cpp.node')

class Program{

  constructor() {
  } // constructor

  init() {

    return new Promise((resolve) => {
      program_init((err, result) => {

        // convert result to JSON
        const json = JSON.parse(result)

        resolve(json)
      })
    })

  } // init

  update(data) {

    // convert data to string
    const string = JSON.stringify(data)

    return new Promise((resolve) => {
      program_update(string, (err, result) => {

        // convert result to JSON
        const json = JSON.parse(result)

        resolve(json)
      })
    })

  } // data

  preview(images, left, right) {

    return new Promise((resolve) => {
      program_preview(images, left, right, (err, result) => {
        resolve(result)
      })
    })

  } // preview

  save() {

    return new Promise((resolve) => {
      program_save((err, result) => {
        resolve(result)
      })
    })

  } // save

} // Program

module.exports = Program;
