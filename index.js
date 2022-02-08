
const {program_data, program_update, program_read, program_quit, program_preview, program_save} = require('./build/Release/emit_from_cpp.node')

class Program{

  constructor() {
  } // constructor

  // main() {
  //   return new Promise((resolve) => {
  //     program_main((err, result) => {
  //       resolve(result)
  //     })
  //   })
  // } // main()

  data() {
    return new Promise((resolve) => {
      program_data((err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  } // data()

  update(data) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_update(string, (err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  } // update(data)

  read(image, left, right, frame_index) {

    return new Promise((resolve) => {
      program_read(images, left, right, frame_index, (err, result) => {
        resolve(result)
      })
    })

  } // read(image, left, right, frame_index)

  quit() {
    return new Promise((resolve) => {
      program_quit((err, result) => {
        resolve(result)
      })
    })
  } // quit()



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
